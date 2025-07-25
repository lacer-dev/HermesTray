#pragma once
#include "SDL3/SDL_error.h"
#include "sys.h"
#include <print>
#include <source_location>
#include <string>

template<class... Args>
inline void eprint(std::format_string<Args...> fmt, Args&&... args)
{
    std::print(stderr, fmt, std::forward<Args>(args)...);
}

template<class... Args>
void eprintln(std::format_string<Args...> fmt, Args&&... args)
{
    std::println(stderr, "{}", std::format(fmt, std::forward<Args>(args)...));
}

enum class Ansi : unsigned
{
    Red = 1,
    Yellow = 3,
    Teal = 6,
    White = 15,
    BrightRed = 196,
};

constexpr std::string colors(Ansi color, const std::string& str)
{
    return std::format("\033[38;5;{}m{}\033]m", static_cast<unsigned>(color), str);
}

constexpr std::string colors(unsigned color, const std::string& str)
{
    return std::format("\033[38;5;{}m{}\033]m", color, str);
}

template<class... Args>
void printmsg(std::string prefix, std::format_string<Args...> fmt, Args&&... args)
{
    eprintln("{}: {}", prefix, std::format(fmt, std::forward<Args>(args)...));
}

inline std::string SDL_PREFIX = colors(Ansi::Teal, "SDL");
inline std::string ERR_PREFIX = colors(Ansi::BrightRed, "error");
inline std::string NOTE_PREFIX = colors(14, "note");
inline std::string WARN_PREFIX = colors(227, "warning");

template<class... Args>
void printerror(std::format_string<Args...> fmt, Args&&... args)
{
    printmsg(ERR_PREFIX, fmt, std::forward<Args>(args)...);
}

namespace Hermes
{

///// Error Generators /////

constexpr std::string prefix_fatal_error(const std::string& message = "")
{
    return "\033[38;5;9merror:\033[m " + message;
}

constexpr std::string prefix_error(const std::string& message = "")
{
    return "\033[38;5;196mfatal error:\033[m " + message;
}

constexpr std::string prefix_warning(const std::string& message = "")
{
    return "\033[38;5;227mwarning:\033[m " + message;
}

constexpr std::string prefix_note(const std::string& message = "")
{
    return "\033[38;5;14mnote:\033[m " + message;
}

constexpr std::string prefix_sdl(const std::string& message = "")
{
    return "\033[38;5;6mSDL:\033[m " + message;
}

constexpr std::string messagebox_title_error()
{
    return processpath().filename().string() + " - Error";
}

constexpr std::string messagebox_title_warning()
{
    return processpath().filename().string() + " - Warning";
}

constexpr std::string messagebox_title_info()
{
    return processpath().filename().string() + " - Information";
}

constexpr std::string directory_not_found(const std::string& directory)
{
    return std::format("directory not found: could not find directory at \"{}\"",
		       directory);
}

constexpr std::string file_not_found(const std::string& file)
{
    return std::format("file not found: could not find file at \"{}\"", file);
}

///// Error Logging Functions /////

}; // namespace Hermes

#ifndef NDEBUG

#define hermes_dbgprint(message) do { println(message); } while (0)
#define hermes_dbgprintln(message) do { eprintln("{}", #message) << '\n'; } while (0)
#define hermes_assert(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            eprintln("{}: " "hermes_assert(\"{}\")\n", colors(9, "failed assertion"), #__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)
#else

#define Hermes_DebugLog(message) ((void)0)
#define Hermes_DebugLogLn(message) ((void)0)
#define Hermes_DebugLogErr(message) ((void)0)
#define Hermes_DebugLogWarn(message) ((void)0)
#define Hermes_DebugLogNote(message) ((void)0)
#define Hermes_Assert(condition) ((void)0)

#endif