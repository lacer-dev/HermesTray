#pragma once
#include <SDL3/SDL.h>
#include <filesystem>
#include <string>

namespace SDL
{

// filesystem
inline std::filesystem::path GetBasePath()
{
    using namespace std::filesystem;

    if (path base{SDL_GetBasePath()}; exists(base))
    {
	return {base};
    }

    return {};
}

// open URLs
inline bool OpenURL(const std::string& url)
{
    return SDL_OpenURL(url.c_str());
}

}; // namespace SDL