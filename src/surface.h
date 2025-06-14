#pragma once
#include <SDL3/SDL.h>

class Surface {
public:
    Surface(int width, int height, SDL_PixelFormat pixelformat = SDL_PIXELFORMAT_RGBA32)
    : _surface(SDL_CreateSurface(width, height, pixelformat)) {
        
    }

    ~Surface() {
        SDL_DestroySurface(_surface);
    }

    SDL_Surface* Handle() {
        return _surface;
    }
private:
    SDL_Surface* _surface;
};