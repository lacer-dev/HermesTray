#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_tray.h>
#include <iostream>
#include <string>
#include "app.h"

/**
 * Initializes SDL and some metadata of the program.
 */
void Intialize();

/**
 * Uninitializes SDL
 */
void Uninitialize();


int main(int argc, char** argv) {
    std::cout << "Hello AFK!\n";
    
    Intialize();
    AFK app = AFK();
    app.Run();
    Uninitialize();

    return 0;
}

void Intialize() {
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING,       "AFK Window");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING,    "0.0.1");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING,    "Leon Allotey");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING,  "Copyright (c) 2025 Leon Allotey");
    // SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,       "application");

    SDL_Init(SDL_INIT_VIDEO);
}

void Uninitialize() {
    SDL_Quit();
}