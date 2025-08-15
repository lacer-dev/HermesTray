#include "pch.h"

#include <chrono>
#include <cstdlib>
#include <expected>
#include <filesystem>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "error.h"
#include "sys.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>
#include <nameof/nameof.hpp>

using namespace hermes;

namespace {
	void handle_unexpected(const std::expected<void, std::string>& expect) {
		if (!expect) {
			logerror("{}", expect.error());
		}
	}

	void show_error_messagebox(const std::string& message) {
		std::string title = this_proc::name() + " - Error";
		static const int button_flags =
			SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT | SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
		std::vector<SDL_MessageBoxButtonData> buttons {{button_flags, 1, "ok"}};

		SDL_MessageBoxData data {
			.flags = SDL_MESSAGEBOX_ERROR | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
			.window = nullptr,
			.title = title.c_str(),
			.message = message.c_str(),
			.numbuttons = static_cast<int>(buttons.size()),
			.buttons = buttons.data(),
			.colorScheme = nullptr};
	}
} // namespace

class Hermes {
public:
	Hermes();
	~Hermes();

	void run();
private:
	// resources
	static constexpr int s_TRAY_ICON_ID = 0;
	ImageLoader m_image_loader;
	void _load_resources();

	// init and exit
	static bool s_active;
	static void _exit(int code);
	static void _restore_global_state();
	static void _set_metadata();

	// callbacks
	static void _callback_toggle_sleep(TrayEntry&);
	static void _callback_quit(TrayEntry&);
	static void _callback_about(TrayEntry&);
};

bool Hermes::s_active = false;

Hermes::Hermes() {
	dbgprintln("querying sdl version... {}.{}.{}", SDL_MAJOR_VERSION, SDL_MICRO_VERSION, SDL_MINOR_VERSION);
	_set_metadata();

	if (!hermes::initialize()) {
		logerror("could not initialize hermes");
	}

	std::atexit(_restore_global_state);
	_load_resources();

	s_active = true;
}

Hermes::~Hermes() {
	// unload resources
	dbgprint("unloading resources... ");
	m_image_loader.unload_all();
	dbgprintln("done");

	_restore_global_state();
}

void Hermes::run() {
	// create systray/notification area data
	dbgprint("creating systray icon... ");
	TrayObject tray {m_image_loader.get(s_TRAY_ICON_ID), "Hermes"};
	dbgprintln("done");
	
	dbgprint("creating systray menu... ");
	TrayMenu menu = tray.new_menu();
	menu.add_label("Quit").set_callback(_callback_quit);
	menu.add_separator();
	menu.add_checkbox("Disable Sleep", !display::can_sleep()).set_callback(_callback_toggle_sleep);
	menu.add_separator();
	menu.add_label("About Hermes").set_callback(_callback_about);
	dbgprintln("done");
	
	// disable sleep when app starts
	display::disable_sleep(true);
	
	// main loop
	dbgprintln("running main loop... running");
	bool running = true;
	while (running) {
		SDL_Event curr_event;
		while (SDL_PollEvent(&curr_event)) {
			if (curr_event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		SDL_Delay(1000 / 10);
	}
	dbgprintln("ending main loop... ended");

	// reenable sleep when app closes
	display::enable_sleep(true);
}

void Hermes::_exit(int code) {
	_restore_global_state();
	exit(code);
}

void Hermes::_load_resources() {
	// load systray icon
	static const std::filesystem::path ICON_PATH = this_proc::dir() / "hermes32.png";
	dbgprint("loading resources... ");
	m_image_loader.load(s_TRAY_ICON_ID, ICON_PATH);
	dbgprintln("done");
}

void Hermes::_restore_global_state() {
	if (s_active) {
		hermes::quit();
		s_active = false;
	}
}

void Hermes::_set_metadata() {
	dbgprint("initializing app metadata... ");
	meta::set_name("HermesTray");
	meta::set_version("0.1.1");
	meta::set_creator("Leon Allotey");
	meta::set_copyright("Copyright (c) 2025 Leon Allotey");
	meta::set_url("https://github.com/lacer-dev/HermesTray");
	meta::set_type(meta::APPLICATION);
	dbgprintln("done");
}

void Hermes::_callback_toggle_sleep(TrayEntry& entry) {
	using namespace display;
	
	if (!(can_sleep() ? disable_sleep() : enable_sleep())) {
		entry.toggle_checked();
		show_error_messagebox("could not toggle sleep");
	}

	dbgassert(entry.is_checked() == !display::can_sleep());
}

void Hermes::_callback_quit(TrayEntry&) {
	dbgprint("sending quit event... ");
	SDL_Event event {SDL_EVENT_QUIT};
	if (!SDL_PushEvent(&event)) {
		dbgprintln("failed");
		logerror("failed to quit properly");
		lognote("sdl: {}", SDL_GetError());
		show_error_messagebox("quit failed");
	}
	dbgprintln("done");
}

void Hermes::_callback_about(TrayEntry&) {
	dbgprint("opening url \"{}\"... ", meta::get_url());
	if (!SDL_OpenURL(meta::get_url().c_str())) {
		dbgprintln("failed");
		logerror("could not open url \"{}\"", meta::get_url());
		lognote("sdl: {}", SDL_GetError());
		show_error_messagebox("could not open url");
		return;
	}
	dbgprintln("done");
}

int main() {
	Hermes app {};
	app.run();
}