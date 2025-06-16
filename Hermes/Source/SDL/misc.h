#pragma once
#include "SDL3/SDL_messagebox.h"
#include <filesystem>

namespace SDL {

// filesystem
std::filesystem::path GetBasePath();

// open URLs
bool OpenURL(const std::string& url);

// messageboxes
inline constexpr int MESSAGEBOX_ERROR = SDL_MESSAGEBOX_ERROR;
inline constexpr int MESSAGEBOX_WARNING = SDL_MESSAGEBOX_WARNING;
inline constexpr int MESSAGEBOX_INFORMATION = SDL_MESSAGEBOX_INFORMATION;
bool ShowSimpleMessageBox(int flags, const std::string& title, const std::string& message);
bool ShowSimpleMessageBoxError(const std::string& title, const std::string& message);
bool ShowSimpleMessageBoxWarning(const std::string& title, const std::string& message);
bool ShowSimpleMessageBoxInformation(const std::string& title, const std::string& message);

};