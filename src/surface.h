#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

namespace afk {

class Surface {
public:
    /**
     * Constructs a black surface with the specified width, height, and pixel format.
     */
    Surface(int width, int height, SDL_PixelFormat pixelformat = SDL_PIXELFORMAT_RGBA32)
    : _surface(SDL_CreateSurface(width, height, pixelformat)) {}

    /**
     * Constructs a surface from an SDL_Surface.
     */
    Surface(SDL_Surface* handle) : _surface(handle) {}

    /**
     * Constructs a surface from an image.
     */
    Surface(const std::filesystem::path& path) 
    : _surface(IMG_Load(path.string().c_str())) {}
    
    ~Surface() {
        SDL_DestroySurface(_surface);
    }

    /**
     * Gets the SDL handle to the surface.
     */
    SDL_Surface* Handle() {
        return _surface;
    }
private:
    SDL_Surface* _surface;
};

};