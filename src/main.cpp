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
		std::string title = this_process::name() + " - Error";
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
	dbg("Querying SDL version{}.{}.{}\n", SDL_MAJOR_VERSION, SDL_MICRO_VERSION, SDL_MINOR_VERSION);
	_set_metadata();

	hermes::global_init();

	std::atexit(_restore_global_state);
	_load_resources();

	s_active = true;
}

Hermes::~Hermes() {
	// unload resources
	dbg("Unloading resources");
	m_image_loader.unload_all();
	dbg(" done\n");

	_restore_global_state();
}

void Hermes::run() {
	// create systray/notification area data
	
	TrayObject tray {m_image_loader.get(s_TRAY_ICON_ID), "Hermes"};dbg(" done\n");
	TrayMenu menu = tray.new_menu();
	menu.add_label("Quit").set_callback(_callback_quit);
	menu.add_separator();
	menu.add_checkbox("Disable Sleep", !display::can_sleep()).set_callback(_callback_toggle_sleep);
	menu.add_separator();
	menu.add_label("About Hermes").set_callback(_callback_about);
	
	// disable sleep when app starts
	display::disable_sleep();
	
	// main loop
	dbg("Running main loop\n");
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
	dbg("Ending main loop\n");

	// reenable sleep when app closes
	display::enable_sleep();
}

void Hermes::_exit(int code) {
	_restore_global_state();
	exit(code);
}

void Hermes::_load_resources() {
	// load systray icon
	static const std::filesystem::path ICON_PATH = this_process::dir() / "hermes32.png";
	dbg("Loading resources");
	m_image_loader.load(s_TRAY_ICON_ID, ICON_PATH);
	dbg(" done\n");
}

void Hermes::_restore_global_state() {
	if (s_active) {
		hermes::global_shutdown();
		s_active = false;
	}
}

void Hermes::_set_metadata() {
	dbg("Initializing app metadata");
	meta::set_name("HermesTray");
	meta::set_version("0.1.1");
	meta::set_creator("Leon Allotey");
	meta::set_copyright("Copyright (c) 2025 Leon Allotey");
	meta::set_url("https://github.com/lacer-dev/HermesTray");
	meta::set_type(meta::APPLICATION);
	dbg(" done\n");
}

void Hermes::_callback_toggle_sleep(TrayEntry& entry) {
	using namespace display;
	dbg("Toggling screensaver:\n ");
	can_sleep() ? disable_sleep() : enable_sleep();
	dbgassert(entry.is_checked() == !display::can_sleep());
}

void Hermes::_callback_quit(TrayEntry&) {
	dbg("Sending quit event...");
	SDL_Event event {SDL_EVENT_QUIT};
	if (!dbgvalidate(SDL_PushEvent(&event))) {
		logerror("Failed to quit properly: SDL: {}", SDL_GetError());
		show_error_messagebox("quit failed");
	}
}

void Hermes::_callback_about(TrayEntry&) {
	dbg("Opening url \"{}\"", meta::get_url());
	if (!dbgvalidate(SDL_OpenURL(meta::get_url().c_str()))) {
		logerror("Failed opening url \"{}\": SDL: {}", meta::get_url(), SDL_GetError());
		show_error_messagebox("could not open url");
		return;
	}
}

int main() {
	Hermes app {};
	app.run();
}