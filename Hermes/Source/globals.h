#pragma once

#include "system.h"
#include "SDLCPP/SDLCPP.h"
#include "error.h"

namespace Hermes {

    inline const std::string EXECUTABLE_NAME = "Hermes.exe";
    inline const std::string METADATA_NAME = "HermesTray";
    inline const std::string METADATA_VERSION = "0.1.1";
    inline const std::string METADATA_CREATOR = "Leon Allotey";
    inline const std::string METADATA_COPYRIGHT = "Copyright (c) 2025 Leon Allotey";
    inline const std::string METADATA_WEBSITE_URL = "https://github.com/lacer-dev/HermesTray";
    inline const std::filesystem::path RESOURCES_DIRECTORY = SDL::GetBasePath();
    inline ResourceManager gResourceManager{RESOURCES_DIRECTORY};

};