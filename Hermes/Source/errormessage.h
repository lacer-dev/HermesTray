#pragma once
#include "globals.h"

namespace Hermes {

constexpr std::string messagebox_title_error() {
    return EXECUTABLE_NAME + " - Error";
}

constexpr std::string messagebox_title_warning() {
    return EXECUTABLE_NAME + " - Warning";
}

constexpr std::string messagebox_title_info() {
    return EXECUTABLE_NAME + " - Information";
}

constexpr std::string directory_not_found(const std::string& directory) {
    return std::format("directory not found: could not find directory at \"{}\"", directory);
}

constexpr std::string file_not_found(const std::string& file) {
    return std::format("file not found: could not find file at \"{}\"", file);
}

}