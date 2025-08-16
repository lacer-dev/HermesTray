#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

// forward declarations
struct SDL_Tray;
struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_Surface;

namespace hermes {
	void global_initialize();
	void global_shutdown();

	namespace this_process {
		// Returns the absolute path of the current process.
		[[nodiscard]] std::filesystem::path path();

		// Returns the parent directory of the current process.
		[[nodiscard]] inline std::filesystem::path directory() { return path().parent_path(); }

		// Returns the file name of the current process.
		[[nodiscard]] inline std::filesystem::path filename() { return path().filename(); }
	} // namespace this_process

	namespace display {
		// Returns `true` if the screensaver is enabled (the display is allowed to sleep) and `false` otherwise.
		// SDL's video subsystem must be initialized before calling this function.
		[[nodiscard]] bool is_screensaver_enabled();

		// Enables the display's screensaver (allowing it to sleep after a period of inactivity). It is enabled by
		// default.
		// SDL's video subsystem must be initialized before calling this function.
		void enable_screensaver();

		// Disables the display's screensaver. It is enabled by default.
		// SDL's video subsystem must be initialized before calling this function.
		void disable_screensaver();
	} // namespace display

	namespace metadata {
		const constexpr std::string APPLICATION {"application"};

		void set_name(const std::string& value);
		void set_version(const std::string& value);
		void set_creator(const std::string& value);
		void set_copyright(const std::string& value);
		void set_url(const std::string& value);
		void set_type(const std::string& value);

		[[nodiscard]] std::string get_url();
	}; // namespace metadata

	// A namespace-scoped timer that executes on the current thread, removing the necessity of creating an independent
	// object for every timing task.
	namespace thread_local_timer {
		using clock_t	   = std::chrono::high_resolution_clock;
		using time_point_t = std::chrono::time_point<clock_t>;

		// Starts/resets the current thread's timer from 0.
		void				 start() noexcept;
		// Stops the current thread's timer. Returns the number of seconds elapsed (to nanosecond precision).
		double				 stop() noexcept;
		// Returns the number of seconds elapsed between the last call to `start()` and the last call to `stop()` (to
		// nanosecond precision).
		[[nodiscard]] double elapsed_seconds() noexcept;
	} // namespace thread_local_timer

	class Image {
	public:
		static Image from_file(const std::filesystem::path&);

		~Image();
		Image()						   = delete;
		Image(const Image&)			   = delete;
		Image& operator=(const Image&) = delete;
		Image(Image&&)				   = default;
		Image& operator=(Image&&)	   = default;
	private:
		friend class TrayObject;

		SDL_Surface* m_handle;

		explicit Image(SDL_Surface* handle) : m_handle {handle} {}
	};

	class TrayEntry {
	public:
		// Warning: Using `std::function` as callback type results in `std::bad_function_call` or segfault when
		// optimizations are enabled.
		using Callback = std::function<void(TrayEntry&)>;

		TrayEntry(const TrayEntry&)			   = delete;
		TrayEntry& operator=(const TrayEntry&) = delete;
		TrayEntry(TrayEntry&&)				   = default;
		TrayEntry& operator=(TrayEntry&&)	   = default;

		TrayEntry set_callback(Callback callback) &&;

		[[nodiscard]] bool is_checked();
		void			   set_checked(bool checked);

		void toggle_checked() { is_checked() ? set_checked(false) : set_checked(true); }
	private:
		friend class TrayMenu;

		SDL_TrayEntry* m_handle;
		Callback	   m_callback;

		explicit TrayEntry(SDL_TrayEntry* handle) : m_handle {handle} {}

		static void invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*);
	};

	class TrayObject;

	class TrayMenu {
	public:
		TrayMenu(const TrayMenu&)			 = delete;
		TrayMenu& operator=(const TrayMenu&) = delete;
		TrayMenu(TrayMenu&&)				 = default;
		TrayMenu& operator=(TrayMenu&&)		 = default;

		TrayEntry add_label(const std::string& label);
		TrayEntry add_checkbox(const std::string& label, bool checked = false);
		TrayEntry add_separator();
	private:
		friend class TrayObject;

		SDL_TrayMenu* m_handle;

		TrayMenu(SDL_TrayMenu* handle) : m_handle {handle} {}

		TrayEntry _insert_entry(int pos, const char* = nullptr, int flags = 0);
	};

	class TrayObject {
	public:
		explicit TrayObject(Image& icon, const std::string& tooltip = {});

		~TrayObject();
		TrayObject(const TrayObject&)			 = delete;
		TrayObject& operator=(const TrayObject&) = delete;
		TrayObject(TrayObject&&)				 = default;
		TrayObject& operator=(TrayObject&&)		 = default;

		TrayMenu new_menu();
	private:
		SDL_Tray* m_handle;
	};
}; // namespace hermes