#pragma once
#include <string>
#include <SDL3/SDL.h>

namespace SDL {


constexpr int INIT_VIDEO = SDL_INIT_VIDEO;

inline bool Init(int systems) {    
    return SDL_Init(systems);
}

inline void Quit() {
    SDL_Quit();
}

inline int WasInit(int systems = 0) { 
    return SDL_WasInit(systems); 
}

inline bool SetMetadataName(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, value.c_str());
}

inline bool SetMetadataVersion(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, value.c_str());
}

inline bool SetMetadataCreator(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, value.c_str());
}

inline bool SetMetadataCopyright(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, value.c_str());
}

inline bool SetMetadataUrl(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, value.c_str());
}

inline bool SetMetadataType(const std::string& value) {
    return SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, value.c_str());
}


};