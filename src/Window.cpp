#include "pch.h"

#include "Window.h"

namespace hermes {

WindowProperties::WindowProperties() : _id{SDL_CreateProperties()} {}

WindowProperties::~WindowProperties() { SDL_DestroyProperties(ID()); }

WindowProperties::WindowProperties(const std::string& title,
								   const int width,
								   const int height)
	: WindowProperties{}
{
	SetTitle(title);
	SetWidth(width);
	SetHeight(height);
}

WindowProperties::WindowProperties(WindowProperties&& other) noexcept
	: _id{std::exchange(other._id, 0)}
{}

WindowProperties& WindowProperties::operator=(WindowProperties&& other) noexcept
{
	if (this != &other)
	{
		SDL_DestroyProperties(this->ID());
		this->_id = std::exchange(other._id, 0);
	}

	return *this;
}

WindowProperties& WindowProperties::SetTitle(const std::string& title)
{
	SDL_SetStringProperty(ID(), SDL_PROP_WINDOW_CREATE_TITLE_STRING,
						  title.c_str());
	return *this;
}

WindowProperties& WindowProperties::SetSize(const int width, const int height)
{
	SetWidth(width);
	SetHeight(height);
	return *this;
}

WindowProperties& WindowProperties::SetWidth(const int width)
{
	SDL_SetNumberProperty(ID(), SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
	return *this;
}

WindowProperties& WindowProperties::SetHeight(const int height)
{
	SDL_SetNumberProperty(ID(), SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
	return *this;
}

////////////////////////////// Window //////////////////////////////

Window::Window(WindowProperties&& properties)
	: _properties{std::move(properties)},
	  _handle{SDL_CreateWindowWithProperties(_properties.ID())}
{}

Window::~Window() { SDL_DestroyWindow(Handle()); }

} // namespace hermes