#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace SDL
{

// messageboxes
inline constexpr int MESSAGEBOX_ERROR = SDL_MESSAGEBOX_ERROR;
inline constexpr int MESSAGEBOX_WARNING = SDL_MESSAGEBOX_WARNING;
inline constexpr int MESSAGEBOX_INFORMATION = SDL_MESSAGEBOX_INFORMATION;

inline bool ShowSimpleMessageBox(int flags, const std::string& title, const std::string& message)
{
    return SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(),
				    nullptr);
}

inline bool ShowSimpleMessageBoxError(const std::string& title,
				      const std::string& message)
{
    return ShowSimpleMessageBox(MESSAGEBOX_ERROR, title, message);
}

inline bool ShowSimpleMessageBoxWarning(const std::string& title,
					const std::string& message)
{
    return ShowSimpleMessageBox(MESSAGEBOX_WARNING, title, message);
}

inline bool ShowSimpleMessageBoxInformation(const std::string& title,
					    const std::string& message)
{
    return ShowSimpleMessageBox(MESSAGEBOX_INFORMATION, title, message);
}

}; // namespace SDL