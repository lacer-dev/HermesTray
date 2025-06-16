#include "../pch.h"
#include "../error.h"
#include "init.h"
#include "misc.h"

namespace SDL {

std::filesystem::path GetBasePath() {
    namespace fs = std::filesystem;
    const auto path = SDL_GetBasePath();
    return (path) ? fs::path{path} : fs::path{};
}

bool OpenURL(const std::string& url) {
    const bool ok = SDL_OpenURL(url.c_str());
    if (!ok)
        err::error(GetError());
    return ok;
}

bool ShowSimpleMessageBox(int flags, const std::string& title, const std::string& message) {
    const bool ok = SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), nullptr);
    if (!ok)
        err::error(GetError());
    return ok;
}

bool ShowSimpleMessageBoxError(const std::string& title, const std::string& message) {
    return ShowSimpleMessageBox(MESSAGEBOX_ERROR, title, message);
}

bool ShowSimpleMessageBoxWarning(const std::string& title, const std::string& message) {
    return ShowSimpleMessageBox(MESSAGEBOX_WARNING, title, message);
}

bool ShowSimpleMessageBoxInformation(const std::string& title, const std::string& message) {
    return ShowSimpleMessageBox(MESSAGEBOX_INFORMATION, title, message);
}


};