#include "pch.h"
#include <chrono>
#include <cstdint>
#include "error.h"
#include "system.h"

namespace {

std::filesystem::path GetProgramPath() {
    const char* path = SDL_GetBasePath();
    if (!path) {
        afk::sys::MessageBoxes::ShowError(SDL_GetError());
        path = "";
    }
    return std::filesystem::path{path};
}

};

namespace afk::sys {

    
//////////////////// MessageBoxes ////////////////////

void MessageBoxes::ShowError(const std::string& message, const std::string& title) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
}


//////////////////// Timing ////////////////////

void Timing::WaitMiliseconds(int duration) {
    uint64_t start = SDL_GetTicks();
    uint64_t end = start + duration;
    while (SDL_GetTicks() < end) {}
}


//////////////////// Display ////////////////////

bool Display::_allow_sleep = []{
    EnableSleeping();
    return true;
}();

void Display::EnableSleeping() {
    int ok = SetThreadExecutionState(ES_CONTINUOUS);
    if (!ok) {
        MessageBoxes::ShowError("Error: Screensaver could not be enabled. Aborting.");
        std::exit(-1);
    }
    _allow_sleep = true;
}

void Display::DisableSleeping() {
    int ok = SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    if (!ok) {
        MessageBoxes::ShowError("Error: Screensaver could not be disabled. Aborting.");
        std::exit(-1);
    }
    _allow_sleep = false;
}

void Display::ToggleSleep() {
    if (IsSleepEnabled()) {
        DisableSleeping();
        Assert(!IsSleepEnabled());
    } else {
        EnableSleeping();
        Assert(IsSleepEnabled());
    }
}


//////////////////// SDLManager //////////////////// 

SDLManager::~SDLManager() {
    Quit();
}

void SDLManager::Init(int systems) {
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING,       "AFK Window");
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING,    "0.0.1");
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING,    "Leon Allotey");
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING,  "Copyright (c) 2025 Leon Allotey");
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING,        "https://github.com/lacer-dev/HermesTray");
    SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,       "application");
    
    if (!SDL_Init(systems)) {
        err::error(err::get_sdl_error());
        MessageBoxes::ShowError();
    }

    std::atexit(SDL_Quit);
}

void SDLManager::Quit() {
    SDL_Quit();
}

std::string SDLManager::GetError() {
    return {SDL_GetError()};
}

void SDLManager::SetAppMetadataProperty(const char*, const char*) {
    bool ok = SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING,       "AFK Window");
    if (!ok) {
        err::error(err::get_sdl_error());
        MessageBoxes::ShowError();
    }
}


//////////////////// ResourceManager ////////////////////

ResourceManager::ResourceManager(const std::string& path_to_res)
: _res_dir{GetProgramPath() / path_to_res} {}

std::filesystem::path ResourceManager::GetPathToResource(const std::string& filename) {
    auto file = _res_dir / filename;
    if (!std::filesystem::exists(file)) {
        std::string message = std::format("file \'{}\' does not exist\n", file.string());
        MessageBoxes::ShowError(message);
        std::exit(ENOENT);
    }

    return file;
}

};