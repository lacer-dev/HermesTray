#include "pch.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>

#include <nameof/nameof.hpp>

#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "error.h"
#include "sys.h"

using namespace hermes;

namespace {
	void show_error_messagebox(const std::string& message) {
		std::string				   title = this_process::filename().generic_string() + " - Error";
		static constexpr const int button_flags =
			SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT | SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
		std::vector<SDL_MessageBoxButtonData> buttons {{button_flags, 1, "ok"}};

		SDL_MessageBoxData data {
			.flags		 = SDL_MESSAGEBOX_ERROR | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
			.window		 = nullptr,
			.title		 = title.c_str(),
			.message	 = message.c_str(),
			.numbuttons	 = static_cast<int>(buttons.size()),
			.buttons	 = buttons.data(),
			.colorScheme = nullptr};
	}

	void open_url(const std::string& url) {
		dbg("Opening URL '{}'...", url);
		if (!dbg_validate(SDL_OpenURL(url.c_str()))) {
			error("Failed to open URL '{}': SDL: {}", url, SDL_GetError());
			show_error_messagebox("Could not open URL");
			return;
		}
	}

	namespace event_queue {
		SDL_Event _current_event;

		SDL_Event* pop() {
			if (!SDL_PollEvent(&_current_event)) {
				return nullptr;
			}
			return &_current_event;
		}

		bool push(const SDL_Event& event) {
			return SDL_PushEvent(const_cast<SDL_Event*>(&event));
		}

	} // namespace event_queue
} // namespace

// App
class Hermes {
public:
	Hermes();
	~Hermes();

	void run();
private:
	// State
	bool m_running = true;

	// Resources
	static const std::filesystem::path TRAY_ICON_PATH;

	// Initialization
	static void set_metadata();

	// Callbacks
	static void callback_toggle_screensaver(TrayEntry&);
	static void callback_quit(TrayEntry&);
	static void callback_about(TrayEntry&);
};

const std::filesystem::path Hermes::TRAY_ICON_PATH = this_process::directory() / "hermes32.png";

Hermes::Hermes() {
	dbg("Querying SDL version... {}.{}.{}\n", SDL_MAJOR_VERSION, SDL_MICRO_VERSION, SDL_MINOR_VERSION);
	set_metadata();
	global_initialize();
 }

Hermes::~Hermes() {
	global_shutdown();
}

void Hermes::run() {
	// Create systray/notification area for Hermes
	Image tray_icon_image = Image::from_file(TRAY_ICON_PATH);
	TrayObject tray {tray_icon_image, "Hermes"};
	TrayMenu   menu = tray.new_menu();
	menu.add_label("Quit").set_callback(callback_quit);
	menu.add_separator();
	menu.add_checkbox("Disable Sleep", !display::is_screensaver_enabled()).set_callback(callback_toggle_screensaver);
	menu.add_separator();
	menu.add_label("About Hermes").set_callback(callback_about);

	// Disable sleep when app starts
	display::disable_screensaver();

	// Main loop
	dbg("Starting main loop\n");
	m_running = true;
	while (m_running) {
		while (SDL_Event* event = event_queue::pop()) {
			if (event->type == SDL_EVENT_QUIT) {
				m_running = false;
				break;
			}
		}

		SDL_Delay(1000 / 10);
	}
	dbg("Ending main loop\n");

	// Re-enable sleep when app closes
	display::enable_screensaver();
}

void Hermes::set_metadata() {
	using namespace metadata;
	
	set_name("HermesTray");
	set_version("0.1.1");
	set_creator("Leon Allotey");
	set_copyright("Copyright (c) 2025 Leon Allotey");
	set_url("https://github.com/lacer-dev/HermesTray");
	set_type(APPLICATION);
}

void Hermes::callback_toggle_screensaver([[maybe_unused]] TrayEntry& entry) {
	using namespace display;

	// Toggle screensaver
	is_screensaver_enabled() ? disable_screensaver() : enable_screensaver();

	// Make sure checkbox is properly checked
	dbg_assert(entry.is_checked() == !is_screensaver_enabled());
}

void Hermes::callback_quit(TrayEntry&) {
	dbg("Quitting after user triggered Quit event\n");

	if (!event_queue::push(SDL_Event {SDL_EVENT_QUIT})) {
		fatal("Failed to quit: SDL: {}", SDL_GetError());
	}
}

void Hermes::callback_about(TrayEntry&) {open_url(metadata::get_url()); }

int main() {
	Hermes app {};
	app.run();
}