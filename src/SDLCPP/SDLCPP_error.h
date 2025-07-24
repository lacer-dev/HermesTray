#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace SDL
{

inline std::string GetError()
{
    return {SDL_GetError()};
}

}; // namespace SDL