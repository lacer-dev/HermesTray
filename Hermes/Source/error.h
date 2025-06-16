#pragma once
#include <string>
#include <iostream>
#include <source_location>

namespace err {

inline std::string source_location_to_string(std::source_location location = std::source_location::current());

inline std::string make_fatal(const std::string& message);
inline std::string make_error(const std::string& message);
inline std::string make_warning(const std::string& message);
inline std::string make_note(const std::string& message);
inline std::string make_sdl(const std::string& message);

inline std::ostream& fatal(const std::string& message);
inline std::ostream& error(const std::string& message);
inline std::ostream& warn(const std::string& message);
inline std::ostream& note(const std::string& message);

std::string source_location_to_string(std::source_location location) {
    return std::string(location.file_name())
        + ':' + std::to_string(location.line()) 
        + ':' + std::to_string(location.column());
}

std::string make_fatal(const std::string& message) {
    return "\033[38;5;196mfatal error:\033[m " + message;
}

std::string make_error(const std::string& message) {
    return "\033[38;5;9merror:\033[m " + message;
}

std::string make_warning(const std::string& message) {
    return "\033[38;5;227mwarning:\033[m " + message;
}

std::string make_note(const std::string& message) {
    return "\033[38;5;14mnote:\033[m " + message;
}

std::string make_sdl(const std::string& message) {
    return "\033[38;5;6mSDL:\033[m " + message;
}

std::ostream& fatal(const std::string& message) {
    return std::cerr << make_fatal(message);
}

std::ostream& error(const std::string& message) {
    return std::cerr << make_error(message);
}

std::ostream& warn(const std::string& message) {
    return std::cerr << make_warning(message);
}

std::ostream& note(const std::string& message) {
    return std::cerr << make_note(message);
}

};

#ifndef NDEBUG
#include <cstdlib>
#include <format>

#define DebugPrint(message) do { std::cout << message; } while (0)
#define DebugPrintLn(message) do { std::cout << message << '\n'; } while (0)
#define DebugPrintError(message) do { err::error(message); } while (0)
#define DebugPrintNote(message) do { err::note(message); } while (0)

#define Assert(condition) do { \
    auto value = condition; \
    if (!value) { \
        err::error(std::format("{}: \033[38;5;9mfailed_assertion\033[m: AssertErr(\"{}\")\n", err::source_location_to_string(), #condition)); \
        std::abort(); \
    } \
} while (0)

#define AssertM(condition, message) do { \
    auto value = condition; \
    if (!value) { \
        err::error(std::format("{}: \033[38;5;9mfailed_assertion\033[m: AssertErr(\"{}\", {})\n", err::source_location_to_string(), #condition, message)); \
        std::abort(); \
    } \
} while (0)

#else

#define DebugPrint(message) ((void)0)
#define DebugPrintLn(message) ((void)0)
#define DebugPrintErr(message) ((void)0)
#define DebugPrintNote(message) ((void)0)
#define Assert(condition) ((void)0)
#define AssertM(condition, message) ((void)0)

#endif