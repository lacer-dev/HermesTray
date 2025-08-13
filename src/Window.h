#pragma once
#include <string>

struct SDL_Window;

namespace hermes {
	class Window {
	public:
		class Properties {
		public:
			using id_type = unsigned;

			Properties();
			Properties(const std::string& title, int width, int height);

			~Properties();
			Properties(const Properties&) = delete;
			Properties(Properties&& other) noexcept;
			Properties& operator=(const Properties&) = delete;
			Properties& operator=(Properties&& other) noexcept;

			id_type get_id() const { return m_id; }

			Properties& set_title(const std::string& title);
			Properties& set_size(int width, int height);
			Properties& set_width(int width);
			Properties& set_height(int height);
		private:
			id_type m_id;
		};

		explicit Window(Properties&& properties);
		~Window();
	private:
		Properties m_properties;
		SDL_Window* m_handle;
	};
} // namespace hermes