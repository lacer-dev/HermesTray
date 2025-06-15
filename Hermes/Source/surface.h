#pragma once
#include <SDL3_image/SDL_image.h>
#include <filesystem>

namespace sdl {

class Surface {
public:
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