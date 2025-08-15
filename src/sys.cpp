#include "pch.h"

#include "sys.h"

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>

#include "error.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_tray.h>
#include <SDL3_image/SDL_image.h>

namespace hermes {
	namespace {
		bool _init_sdl_subsystem(int subsystem_flag, std::string subsystem_name) {
			dbg("Initializing SDL {}... ", subsystem_name);
			dbgdo(thread_local_timer::start());

			if (!dbgvalidate(SDL_InitSubSystem(subsystem_flag), "")) {
				fatal_error("Failed to initialize SDL {} subsystem: SDL: {}", subsystem_name, SDL_GetError());
				return false;
			}
			
			dbg(" done ({}s)\n", thread_local_timer::stop());
			return true;
		}
	} // namespace

	void global_init() {
		_init_sdl_subsystem(SDL_INIT_EVENTS, "events");
		_init_sdl_subsystem(SDL_INIT_VIDEO, "video");
	}

	void global_shutdown() {
		dbg("quitting sdl... ");
		SDL_Quit();
		dbg(" done\n");
	}

	namespace display {
		void enable_sleep() {
			dbg("Enabling screensaver... ");
			if (!dbgvalidate(SDL_EnableScreenSaver(), "on\n")) {
				fatal_error("Failed to enable screensaver: SDL: {}", SDL_GetError());
			}
		}

		void disable_sleep() {
			dbg("Disabling screensaver... ");
			if (!dbgvalidate(SDL_DisableScreenSaver(), "off\n")) {
				fatal_error("Failed to disable screensaver: SDL: {}", SDL_GetError());
			}
			dbg("off");
		}

		bool can_sleep() {
			return SDL_ScreenSaverEnabled();
		}
	} // namespace display
	namespace meta {
		namespace {
			std::invalid_argument _invalid_metadata_property(const std::string& property, const std::string& value) {
				return std::invalid_argument {
					std::format("\"{}\" is not a valid value for metadata property \"{}\"", value, property)};
			}
		} // namespace

		void set_name(const std::string& value) {
			SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, value.c_str());
		}

		void set_version(const std::string& value) {
			SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, value.c_str());
		}

		void set_creator(const std::string& value) {
			SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, value.c_str());
		}

		void set_copyright(const std::string& value) {
			SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, value.c_str());
		}

		void set_url(const std::string& value) {
			SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, value.c_str());
		}

		void set_type(const std::string& value) {
			try {
				SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, value.c_str());
			} catch (const std::invalid_argument&) {
				throw _invalid_metadata_property("type", value);
			}
		}

		std::string get_url() {
			return {SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING)};
		}
	} // namespace meta

	namespace thread_local_timer {
		namespace {
			thread_local time_point_t _start;
			thread_local time_point_t _end;
			thread_local bool _on;
		} // namespace

		// resets and starts timer
		void start() {
			_on = true;
			_start = clock_t::now();
		}

		// stop the timer
		// if timer is stopped, does nothing
		double stop() {
			if (_on) {
				_end = clock_t::now();
				_on = false;
			}

			return elapsed();
		}

		double elapsed() {
			time_point_t end_time = (_on) ? clock_t::now() : _end;
			std::chrono::nanoseconds time_elapsed = end_time - _start;
			return time_elapsed.count() / 1e9;
		}
	} // namespace thread_local_timer

	TrayEntry& TrayEntry::set_callback(Callback callback) {
		m_callback = callback;
		SDL_SetTrayEntryCallback(m_handle, _invoke_entry_callback, this);
		return *this;
	}

	bool TrayEntry::is_checked() {
		return SDL_GetTrayEntryChecked(m_handle);
	}

	void TrayEntry::set_checked(bool checked) {
		SDL_SetTrayEntryChecked(m_handle, checked);
	}

	void TrayEntry::_invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*) {
		auto& ref_entry = *static_cast<TrayEntry*>(ptr_entry);
		ref_entry.m_callback(ref_entry);
	}

	TrayEntry TrayMenu::add_label(const std::string& label) {
		return _insert_entry(0, label.c_str());
	}

	TrayEntry TrayMenu::add_checkbox(const std::string& label, bool checked) {
		return _insert_entry(0, label.c_str(), SDL_TRAYENTRY_CHECKBOX | (checked ? SDL_TRAYENTRY_CHECKED : 0));
	}

	TrayEntry TrayMenu::add_separator() {
		return _insert_entry(0);
	}

	TrayEntry TrayMenu::_insert_entry(int position, const char* label_c_str, int flags) {
		auto raw_handle = SDL_InsertTrayEntryAt(m_handle, position, label_c_str, flags);
		if (raw_handle == nullptr) {
			throw std::out_of_range(std::format("systray entry position (which is {}) is out of bounds", position));
		}

		return TrayEntry {raw_handle};
	}

	SDL_Tray* _new_tray_handle(SDL_Surface* image, const char* tooltip) {
		dbg("Creating systray icon...");
		SDL_Tray* handle = SDL_CreateTray(image, tooltip);
		if (!dbgvalidate(handle)) {
			fatal_error("Failed to create a systray icon: SDL: {}", SDL_GetError());
		}
		return handle;
	}

	TrayObject::TrayObject(Image& icon, const std::string& tooltip)
		: m_handle {_new_tray_handle(icon.m_handle, tooltip.c_str())} {
	}

	TrayObject::~TrayObject() {
		SDL_DestroyTray(m_handle);
	}

	TrayMenu TrayObject::new_menu() {
		dbg("Creating systray menu...");
		SDL_TrayMenu* menu_handle = SDL_CreateTrayMenu(m_handle);
		if (!dbgvalidate(menu_handle)) {
			fatal_error("Failed to create a systray menu: SDL: {}", SDL_GetError());
		}
		return TrayMenu {menu_handle};
	}

	Image Image::from_file(const std::filesystem::path& path) {
		if (!std::filesystem::exists(path)) {
			throw std::system_error(
				std::make_error_code(std::errc::no_such_file_or_directory),
				std::format("file at \"{}\" does not exist", path.string()));
		}

		auto handle = IMG_Load(path.string().c_str());
		if (!handle) {
			throw std::invalid_argument(SDL_GetError());
		}

		return Image {handle};
	}

	Image::~Image() {
		SDL_DestroySurface(m_handle);
	}

	ImageLoader::~ImageLoader() {
		unload_all();
	}

	void ImageLoader::load(Id id, const std::filesystem::path& file) {
		m_images_map.insert({id, std::make_unique<Image>(Image::from_file(file))});
	}

	Image& ImageLoader::get(Id id) {
		if (!m_images_map.contains(id)) {
			throw std::out_of_range(std::format("resource with id {} does not exist", id));
		}

		return *m_images_map[id];
	}
}; // namespace hermes