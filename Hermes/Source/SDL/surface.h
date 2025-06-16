#pragma once
#include <filesystem>

struct SDL_Surface;

namespace SDL {

class Surface {
public:
    /**
     * Constructs a surface from a handle.
     */
    Surface(SDL_Surface* handle);
    
    Surface(const Surface&) = delete;
    Surface(Surface&& other);

    ~Surface();

    /**
     * Gets the SDL handle to the surface.
     */
    SDL_Surface* Handle();
private:
    SDL_Surface* _handle;
};

namespace IMG {
    
/**
* Constructs a surface from an image.
*/
Surface Load(const std::filesystem::path& path);

};

};