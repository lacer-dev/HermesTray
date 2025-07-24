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
    else
    {
	return {};
    }
}

}; // namespace SDL