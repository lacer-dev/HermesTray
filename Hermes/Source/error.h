#pragma once
#include <string>
#include <iostream>
#include <source_location>

namespace err {

std::string source_location_to_string(std::source_location location = std::source_location::current());
std::string make_error(const std::string& message);
std::string make_note(const std::string& message);
std::ostream& error(const std::string& message);
std::ostream& note(const std::string& message);

#ifdef SDL_error_h_ 
std::string get_sdl_error();
#endif

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
#define DebugPrintLn(message ((void)0))
#define DebugPrintErr(message) ((void)0)
#define DebugPrintNote(message) ((void)0)
#define Assert(condition) ((void)0)
#define AssertM(condition, message) ((void)0)

#endif