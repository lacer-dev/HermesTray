#include "pch.h"

#include "Window.h"

namespace hermes {
	Window::Properties::Properties() : m_id {SDL_CreateProperties()} {}

	Window::Properties::~Properties() { SDL_DestroyProperties(get_id()); }

	Window::Properties::Properties(const std::string& title, const int width, const int height)
		: Window::Properties {} {
		set_title(title);
		set_width(width);
		set_height(height);
	}

	Window::Properties::Properties(Window::Properties&& other) noexcept : m_id {std::exchange(other.m_id, 0)} {}

	Window::Properties& Window::Properties::operator=(Window::Properties&& other) noexcept {
		this->m_id = std::exchange(other.m_id, 0);
		return *this;
	}

	Window::Properties& Window::Properties::set_title(const std::string& title) {
		SDL_SetStringProperty(get_id(), SDL_PROP_WINDOW_CREATE_TITLE_STRING, title.c_str());
		return *this;
	}

	Window::Properties& Window::Properties::set_size(const int width, const int height) {
		return set_width(width), set_height(height);
	}

	Window::Properties& Window::Properties::set_width(const int width) {
		SDL_SetNumberProperty(get_id(), SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
		return *this;
	}

	Window::Properties& Window::Properties::set_height(const int height) {
		SDL_SetNumberProperty(get_id(), SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
		return *this;
	}

	////////////////////////////// Window //////////////////////////////

	Window::Window(Window::Properties&& properties)
		: m_properties {std::move(properties)},
		  m_handle {SDL_CreateWindowWithProperties(m_properties.get_id())} {}

	Window::~Window() { SDL_DestroyWindow(m_handle); }
} // namespace hermes