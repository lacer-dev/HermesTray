#include "init.h"
#include <SDL3/SDL.h>
#include <stdexcept>
#include "../error.h"
#include "../system.h"

namespace {

bool SetAppMetadataProperty(const char* property, const char* value) {
    const bool ok = SDL_SetAppMetadataProperty(property, value);
    if (!ok)
        err::error(SDL::GetError());
    return ok;
}

};

namespace SDL {

bool Init(int systems) {    
    const bool ok = SDL_Init(systems);
    if (!ok)
        err::error(GetError());
    return ok;
}

void Quit() {
    SDL_Quit();
}

int WasInit(int flags) { return SDL_WasInit(flags); }

std::string GetError() { return {SDL_GetError()}; }

bool SetMetadataName(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, value.c_str());
    
}

bool SetMetadataVersion(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, value.c_str());
    
}

bool SetMetadataCreator(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, value.c_str());
    
}

bool SetMetadataCopyright(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, value.c_str());
    
}

bool SetMetadataUrl(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, value.c_str());
    
}

bool SetMetadataType(const std::string& value) {
    return SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, value.c_str());
    
}

};