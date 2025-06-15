#include "pch.h"
#include "error.h"

namespace err {

std::string source_location_to_string(std::source_location location) {
    return std::string(location.file_name())
        + ':' + std::to_string(location.line()) 
        + ':' + std::to_string(location.column());
}

std::string make_error(const std::string& message) {
    return "\033[38;5;9merror:\033[m " + message;
}

std::string make_note(const std::string& message) {
    return "\033[38;5;14mnote:\033[m " + message;
}

std::ostream& error(const std::string& message) {
    return std::cerr << make_error(message);
}

std::ostream& note(const std::string& message) {
    return std::cerr << make_note(message);
}

#ifdef SDL_error_h_ 

#include <SDL3/SDL_error.h>

std::string get_sdl_error() {
    return std::string("\033[38;5;6mSDL:\033[m ") + SDL_GetError();
}

#endif
};