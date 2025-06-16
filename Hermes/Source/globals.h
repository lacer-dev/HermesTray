#include "system.h"
#include "SDL/SDLcpp.h"
#include "error.h"

namespace hermes {

inline std::filesystem::path RESOURCES_DIRECTORY = []{
    const auto exe_directory = SDL::GetBasePath();
    if (exe_directory.empty()) {
        err::fatal(SDL::GetError());
        std::terminate();
    }
    return exe_directory;
}();

inline ResourceManager gResourceManager{RESOURCES_DIRECTORY};
inline constexpr std::string APP_NAME = "hermes.exe";
inline const std::string MESSAGEBOX_TITLE_ERROR = hermes::APP_NAME + " - Error";
inline const std::string WEBSITE_URL = "https://github.com/lacer-dev/HermesTray";

};