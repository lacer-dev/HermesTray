#include "system.h"
#include "error.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_messagebox.h>

namespace {

std::filesystem::path GetProgramPath() {
    return std::filesystem::path{SDL_GetBasePath()};
}

};

namespace afk {

void ShowErrorMessageBox(const std::string& message) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "AFK Window Error",
        message.c_str(),
        nullptr
    );
}


bool ToggleScreensaver() {
    bool screensaver_on = SDL_ScreenSaverEnabled();
    if (screensaver_on) {
        if (!SDL_DisableScreenSaver()) {
            ShowErrorMessageBox("Error: Screensaver could not be disabled. Aborting.");
            std::exit(-1);
        }
        
        Assert(SDL_ScreenSaverEnabled() == false);
    } else {
        if (!SDL_EnableScreenSaver()) {
            ShowErrorMessageBox("Error: Screensaver could not be enabled. Aborting.");
            std::exit(-1);
        }

        Assert(SDL_ScreenSaverEnabled() == true);
    }

    return !screensaver_on;
}


std::filesystem::path GetFileResource(const std::string& filename) {
    auto filePath = (GetProgramPath() / "Resources") / filename;
    if (std::filesystem::is_regular_file(filePath)) {
        return filePath;
    } else {
        auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
        std::string message = std::format("file \'{}\' does not exist\n", filePath.string());
        throw std::system_error(errorCode, message);
    }
}

};