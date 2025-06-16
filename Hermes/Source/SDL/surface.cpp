#include "../pch.h"
#include <utility>
#include "surface.h"

namespace SDL {

Surface::Surface(SDL_Surface* handle) 
: _handle{handle} {}

Surface::Surface(Surface&& other) 
: _handle(std::exchange(other._handle, nullptr)) {}

Surface::~Surface() {
    SDL_DestroySurface(Handle());
}

SDL_Surface* Surface::Handle() {
    return _handle;
}   

};

namespace SDL::IMG {

Surface Load(const std::filesystem::path& path) {
    return Surface{IMG_Load(path.string().c_str())};
}

};