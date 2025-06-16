#include "SDL/init.h"
#include "pch.h"
#include <chrono>
#include <cstdint>
#include "error.h"
#include "system.h"
#include "SDL/SDLcpp.h"
#include "globals.h"

//////////////////// Display ////////////////////

namespace {
bool _display_is_allowed_to_sleep = true;
};

namespace hermes {

bool IsDisplaySleepEnabled() { return _display_is_allowed_to_sleep; }
bool IsDisplaySleepDisabled() { return !IsDisplaySleepEnabled(); }

void EnableDisplaySleep() {
    static const std::string ERROR_SCREENSAVER_NOT_ENABLED = "screensaver could not be enabled";

    int ok = SetThreadExecutionState(ES_CONTINUOUS);
    if (!ok) {
        err::fatal(ERROR_SCREENSAVER_NOT_ENABLED);
        if (SDL::WasInit())
            SDL::ShowSimpleMessageBoxError(MESSAGEBOX_TITLE_ERROR, ERROR_SCREENSAVER_NOT_ENABLED);
        std::terminate();
    }
    _display_is_allowed_to_sleep = true;
}

void DisableDisplaySleep() {
    static const std::string ERROR_SCREENSAVER_NOT_DISABLED = "screensaver could not be disabled";
    
    int ok = SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    if (!ok) {
        err::fatal(ERROR_SCREENSAVER_NOT_DISABLED);
        if (SDL::WasInit())
            SDL::ShowSimpleMessageBoxError(MESSAGEBOX_TITLE_ERROR, ERROR_SCREENSAVER_NOT_DISABLED);
        std::terminate();
    }
    _display_is_allowed_to_sleep = false;
}

void ToggleDisplaySleep() {
    if (IsDisplaySleepEnabled()) {
        DisableDisplaySleep();
        Assert(!IsDisplaySleepEnabled());
    } else {
        EnableDisplaySleep();
        Assert(IsDisplaySleepEnabled());
    }
}

};


namespace hermes {

//////////////////// Timing ////////////////////

void WaitMiliseconds(int duration) {
    uint64_t start = SDL_GetTicks();
    uint64_t end = start + duration;
    while (SDL_GetTicks() < end) {}
}


//////////////////// ResourceManager ////////////////////

ResourceManager::ResourceManager(const std::filesystem::path& resources_directory) {
    namespace fs = std::filesystem;
    if (!fs::exists(resources_directory)) {
        const std::string error_message = std::format("directory not found: could not find directory at \"{}\"", resources_directory.string());
        err::fatal(error_message + '\n');
        if (SDL::WasInit())
            SDL::ShowSimpleMessageBoxError(MESSAGEBOX_TITLE_ERROR, error_message);
        std::terminate();
    }
    
    _directory = resources_directory;
}

std::filesystem::path ResourceManager::GetPath(const std::string& filename) {
    const auto file = Dir() / filename;
    if (!std::filesystem::exists(file)) {
        const std::string error_message = std::format("file not found: could not find file at \"{}\"", file.string());
        err::error(error_message + '\n');
        if (SDL::WasInit())
            SDL::ShowSimpleMessageBoxError(MESSAGEBOX_TITLE_ERROR, error_message);
    }

    return file;
}

};