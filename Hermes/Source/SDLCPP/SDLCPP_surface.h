#pragma once
#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace SDL {

class Surface {
public:
    /**
     * Constructs a surface from a handle.
     */
    Surface(SDL_Surface* handle) : _handle{handle} {}
    Surface(const Surface&) = delete;
    Surface(Surface&& other) : _handle(std::exchange(other._handle, nullptr)) {}

    ~Surface() {
        SDL_DestroySurface(Handle());
    }

    /**
     * Gets the SDL handle to the surface.
     */
    SDL_Surface* Handle() {
        return _handle;
    }

    bool IsNull() {
        return _handle == nullptr;
    }

    operator bool() {
        return !IsNull();
    }
private:
    SDL_Surface* _handle;
};

namespace IMG {
    
/**
* Constructs a surface from an image.
*/
inline Surface Load(const std::filesystem::path& path) {
    return {IMG_Load(path.string().c_str())};
}

};

};