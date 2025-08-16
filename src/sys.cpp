#include "pch.h"

#include "sys.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_tray.h>
#include <SDL3_image/SDL_image.h>

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

#include "error.h"

namespace hermes {
	namespace {
		bool _initialize_sdl_subsystem(const int subsystem_flag, const std::string_view subsystem_name) {
			dbg("Initializing SDL {} subsystem...", subsystem_name);
#ifndef NDEBUG
			thread_local_timer::start();
#endif
			if (!dbg_validate(SDL_InitSubSystem(subsystem_flag), "")) {
				fatal("Failed to initialize SDL {} subsystem: SDL: {}", subsystem_name, SDL_GetError());
			}

			dbg(" done ({}s)\n", thread_local_timer::stop());
			return true;
		}
	} // namespace

	void global_initialize() {
		std::set_terminate(global_shutdown);
		_initialize_sdl_subsystem(SDL_INIT_EVENTS, "event");
		_initialize_sdl_subsystem(SDL_INIT_VIDEO, "video");
	}

	void global_shutdown() {
		dbg("Quitting SDL...");
		SDL_Quit();
		dbg(" done\n");
	}

	namespace display {
		void enable_screensaver() {
			dbg("Enabling screensaver...");
			if (!dbg_validate(SDL_EnableScreenSaver(), " on\n")) {
				fatal("Failed to enable screensaver: SDL: {}", SDL_GetError());
			}
		}

		void disable_screensaver() {
			dbg("Disabling screensaver...");
			if (!dbg_validate(SDL_DisableScreenSaver(), " off\n")) {
				fatal("Failed to disable screensaver: SDL: {}", SDL_GetError());
			}
		}

		bool is_screensaver_enabled() { return SDL_ScreenSaverEnabled(); }
	} // namespace display

	namespace metadata {
		namespace {
			void _set_metadata_property(
				const std::string_view property_name,
				const char*			   sdl_property_id,
				const std::string&	   value) {
				eprint("Setting metadata property '{}'...", property_name);
				bool success = SDL_SetAppMetadataProperty(sdl_property_id, value.c_str());
				if (!dbg_validate(success)) {
					std::string error_message =
						std::format("Failed to set metadata property '{}': SDL: {}", property_name, SDL_GetError());
					throw std::invalid_argument {error_message};
				}
			}
		} // namespace

		void set_name(const std::string& value) {
			_set_metadata_property("name", SDL_PROP_APP_METADATA_NAME_STRING, value);
		}

		void set_version(const std::string& value) {
			_set_metadata_property("version", SDL_PROP_APP_METADATA_VERSION_STRING, value);
		}

		void set_creator(const std::string& value) {
			_set_metadata_property("creator", SDL_PROP_APP_METADATA_CREATOR_STRING, value);
		}

		void set_copyright(const std::string& value) {
			_set_metadata_property("copyright", SDL_PROP_APP_METADATA_COPYRIGHT_STRING, value);
		}

		void set_url(const std::string& value) {
			_set_metadata_property("url", SDL_PROP_APP_METADATA_URL_STRING, value);
		}

		void set_type(const std::string& value) {
			_set_metadata_property("type", SDL_PROP_APP_METADATA_TYPE_STRING, value);
		}

		std::string get_url() {
			const char* str = SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING);
			if (!str) {
				return {};
			}
			return str;
		}
	} // namespace metadata

	namespace thread_local_timer {
		namespace {
			thread_local time_point_t _tlt_time_start;
			thread_local time_point_t _tlt_time_end;
			thread_local bool		  _tlt_on;
		} // namespace

		void start() noexcept {
			_tlt_on			= true;
			_tlt_time_start = clock_t::now();
		}

		double stop() noexcept {
			_tlt_time_end = clock_t::now();
			_tlt_on		  = false;
			return elapsed_seconds();
		}

		double elapsed_seconds() noexcept {
			time_point_t			 end_time	  = (_tlt_on) ? clock_t::now() : _tlt_time_end;
			std::chrono::nanoseconds time_elapsed = end_time - _tlt_time_start;
			return time_elapsed.count() / 1e9;
		}
	} // namespace thread_local_timer

	TrayEntry TrayEntry::set_callback(Callback callback) && {
		m_callback = callback;
		SDL_SetTrayEntryCallback(m_handle, invoke_entry_callback, this);
		return std::move(*this);
	}

	bool TrayEntry::is_checked() { return SDL_GetTrayEntryChecked(m_handle); }

	void TrayEntry::set_checked(bool checked) { SDL_SetTrayEntryChecked(m_handle, checked); }

	void TrayEntry::invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*) {
		auto& ref_entry = *static_cast<TrayEntry*>(ptr_entry);
		ref_entry.m_callback(ref_entry);
	}

	TrayEntry TrayMenu::add_label(const std::string& label) { return _insert_entry(0, label.c_str()); }

	TrayEntry TrayMenu::add_checkbox(const std::string& label, bool checked) {
		const int flags = SDL_TRAYENTRY_CHECKBOX | (checked ? SDL_TRAYENTRY_CHECKED : 0);
		return _insert_entry(0, label.c_str(), flags);
	}

	TrayEntry TrayMenu::add_separator() { return _insert_entry(0); }

	TrayEntry TrayMenu::_insert_entry(int position, const char* label_c_str, int flags) {
		const auto raw_handle = SDL_InsertTrayEntryAt(m_handle, position, label_c_str, flags);
		if (raw_handle == nullptr) {
			throw std::out_of_range(std::format("systray entry position (which is {}) is out of bounds", position));
		}

		return TrayEntry {raw_handle};
	}

	SDL_Tray* _new_tray_handle(SDL_Surface* image, const char* tooltip) {
		dbg("Creating systray icon...");
		SDL_Tray* handle = SDL_CreateTray(image, tooltip);
		if (!dbg_validate(handle)) {
			fatal("Failed to create a systray icon: SDL: {}", SDL_GetError());
		}
		return handle;
	}

	TrayObject::TrayObject(Image& icon, const std::string& tooltip)
		: m_handle {_new_tray_handle(icon.m_handle, tooltip.c_str())} {}

	TrayObject::~TrayObject() { SDL_DestroyTray(m_handle); }

	TrayMenu TrayObject::new_menu() {
		dbg("Creating systray menu...");
		SDL_TrayMenu* menu_handle = SDL_CreateTrayMenu(m_handle);
		if (!dbg_validate(menu_handle)) {
			fatal("Failed to create a systray menu: SDL: {}", SDL_GetError());
		}
		return TrayMenu {menu_handle};
	}

	Image Image::from_file(const std::filesystem::path& file) {
		dbg("Loading image from \'{}\'...", file.filename().generic_string());
		if (!dbg_validate(std::filesystem::exists(file), "")) {
			auto error_code	   = std::make_error_code(std::errc::no_such_file_or_directory);
			auto error_message = std::format("File '{}' does not exist", file.generic_string());
			throw std::system_error(error_code, error_message);
		}
		
		SDL_Surface* handle = IMG_Load(file.string().c_str());
		if (!dbg_validate(handle)) {
			throw std::invalid_argument(std::format("Failed to load image: SDL: {}", SDL_GetError()));
		}
		
		return Image {handle};
	}

	Image::~Image() { SDL_DestroySurface(m_handle); }
}; // namespace hermes