#pragma once

#include <algorithm>
#include <expected>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

// forward declarations
struct SDL_Tray;
struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_Surface;

namespace hermes {
	bool initialize();
	void quit();

	namespace this_proc {
		const std::filesystem::path& path();
		inline std::filesystem::path dir() {
			return path().parent_path();
		}
		inline std::string name() {
			return path().filename().string();
		}
	} // namespace this_proc

	namespace display {
		bool can_sleep();
		bool enable_sleep(bool perror = false);
		bool disable_sleep(bool perror = false);
	} // namespace display

	namespace meta {
		const constexpr std::string APPLICATION {"application"};

		void set_name(const std::string& value);
		void set_version(const std::string& value);
		void set_creator(const std::string& value);
		void set_copyright(const std::string& value);
		void set_url(const std::string& value);
		void set_type(const std::string& value);

		std::string get_url();

		struct AppMetadata {
			const std::string name;
			const std::string version;
			const std::string creator;
			const std::string copyright;
			const std::string url;
		};

		inline void set_all(const AppMetadata& value) {
			if (!value.name.empty()) {
				set_name(value.name);
			}

			if (!value.version.empty()) {
				set_version(value.version);
			}

			if (!value.creator.empty()) {
				set_creator(value.creator);
			}

			if (!value.copyright.empty()) {
				set_copyright(value.copyright);
			}

			if (!value.url.empty()) {
				set_url(value.url);
			}
		}
	}; // namespace meta

	namespace thread_local_timer {
		using clock_t = std::chrono::high_resolution_clock;
		using time_point_t = std::chrono::time_point<clock_t>;

		void start();
		double stop();
		double elapsed();
	} // namespace thread_local_timer

	class Image {
	public:
		Image() = delete;
		static Image from_file(const std::filesystem::path&);

		Image(const Image&) = delete;
		Image(Image&&) = default;
		Image& operator=(const Image&) = default;
		Image& operator=(Image&&) = default;
		~Image();
	private:
		friend class TrayObject;

		SDL_Surface* m_handle;

		explicit Image(SDL_Surface* handle)
			: m_handle {handle} {}
	};

	class ImageLoader {
	public:
		using Id = unsigned;

		~ImageLoader();

		void load(Id id, const std::filesystem::path& path);
		Image& get(Id id);

		void unload_all() { m_images_map.clear(); }

		std::size_t count() { return m_images_map.size(); }
	private:
		std::unordered_map<Id, std::unique_ptr<Image>> m_images_map;
	};

	class TrayEntry {
	public:
		// Note: Using `std::function` as callback type results in `std::bad_function_call` or segfault when
		// optimizations are enabled.
		using Callback = std::function<void(TrayEntry&)>;
		// using Callback = void(*)(TrayEntry&);

		TrayEntry(const TrayEntry&) = delete;
		TrayEntry(TrayEntry&&) = default;

		TrayEntry& set_callback(Callback callback);

		// checkbox
		bool is_checked();
		void set_checked(bool checked);
		void toggle_checked() { is_checked() ? set_checked(false) : set_checked(true); }
	private:
		friend class TrayMenu;

		SDL_TrayEntry* m_handle;
		Callback m_callback;

		explicit TrayEntry(SDL_TrayEntry* handle)
			: m_handle {handle} {}

		static void _invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*);
	};

	class TrayObject;

	class TrayMenu {
	public:
		TrayMenu(const TrayMenu&) = delete;
		TrayMenu(TrayMenu&&) = default;

		TrayEntry add_label(const std::string& label);
		TrayEntry add_checkbox(const std::string& label, bool checked = false);
		TrayEntry add_separator();
	private:
		friend class TrayObject;

		SDL_TrayMenu* m_handle;

		TrayMenu(SDL_TrayMenu* handle)
			: m_handle {handle} {}

		TrayEntry _insert_entry(int pos, const char* = nullptr, int flags = 0);
	};

	class TrayObject {
	public:
		explicit TrayObject(Image& icon, const std::string& tooltip = {});
		~TrayObject();

		TrayMenu new_menu();
	private:
		SDL_Tray* m_handle;
	};
}; // namespace hermes