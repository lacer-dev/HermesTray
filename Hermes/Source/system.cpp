#include "pch.h"
#include "SDLCPP/SDLCPP.h"
#include "error.h"
#include "system.h"
#include "globals.h"
#include "errormessage.h"

namespace Hermes {

//////////////////// ResourceManager ////////////////////

ResourceManager::ResourceManager(const std::filesystem::path& resources_directory) {
    using namespace std::filesystem;

    if (!exists(resources_directory))
        SDL::ShowSimpleMessageBoxError(messagebox_title_error(), directory_not_found(resources_directory.string()));
    
    _directory = resources_directory;
}

std::filesystem::path ResourceManager::GetResource(const std::string& filename) {
    using namespace std::filesystem;

    const path file = Directory() / filename;
    if (!exists(file))
        SDL::ShowSimpleMessageBoxError(messagebox_title_error(), file_not_found(file.string()));

    return file;
}

};