#pragma once
#include <string>

struct SDL_Window;

namespace Hermes
{

class WindowProperties
{
    using Self = WindowProperties;

public:
    using id_type = unsigned;

    WindowProperties();
    WindowProperties(const std::string& title, int width, int height);

    ~WindowProperties();
    WindowProperties(const WindowProperties&) = delete;
    WindowProperties(WindowProperties&& other) noexcept;
    WindowProperties& operator=(const WindowProperties&) = delete;
    WindowProperties& operator=(WindowProperties&& other) noexcept;

    id_type ID() const { return _id; }

    Self& SetTitle(const std::string& title);
    Self& SetSize(int width, int height);
    Self& SetWidth(int width);
    Self& SetHeight(int height);

private:
    id_type _id;
};

class Window
{
public:
    using handle_type = SDL_Window;

    explicit Window(WindowProperties&& properties);
    ~Window();

    handle_type* Handle() const { return _handle; }

private:
    WindowProperties _properties;
    handle_type* _handle;
};

} // namespace Hermes