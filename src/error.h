#pragma once
#include "SDL3/SDL_error.h"
#include "globals.h"
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

namespace Hermes
{

///// Error Generators /////

constexpr std::string current_source_location_str(std::source_location location = std::source_location::current())
{
    return std::string(location.file_name()) + ':' + std::to_string(location.line()) + ':' + std::to_string(location.column());
}

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

constexpr std::string prefix_sdl_error(const std::string& message = "")
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
    return std::format("directory not found: could not find directory at \"{}\"", directory);
}

constexpr std::string file_not_found(const std::string& file)
{
    return std::format("file not found: could not find file at \"{}\"", file);
}

///// Error Logging Functions /////

inline void logfatalerror(const std::string& message)
{
    eprintln("{}{}", prefix_fatal_error(), message);
}

inline void logerror(const std::string& message)
{
    eprintln("{}{}", prefix_error(), message);
}

inline void logwarn(const std::string& message)
{
    eprintln("{}{}", prefix_warning(), message);
}

inline void lognote(const std::string& message)
{
    eprintln("{}{}", prefix_note(), message);
}

inline void log_last_sdl_error()
{
    eprintln("{}{}", prefix_sdl_error(), SDL_GetError());
}

}; // namespace Hermes

#ifndef NDEBUG

#    define Hermes_DebugLog(message) \
	do                           \
	{                            \
	    std::cerr << message;    \
	}                            \
	while (0)
#    define Hermes_DebugLogLn(message)    \
	do                                \
	{                                 \
	    std::cerr << message << '\n'; \
	}                                 \
	while (0)
#    define Hermes_DebugLogError(message) \
	do                                \
	{                                 \
	    Hermes::logerror(message);    \
	}                                 \
	while (0)
#    define Hermes_DebugLogWarn(message) \
	do                               \
	{                                \
	    Hermes::logwarn(message);    \
	}                                \
	while (0)
#    define Hermes_DebugLogNote(message) \
	do                               \
	{                                \
	    Hermes::lognote(message);    \
	}                                \
	while (0)

#    define Hermes_Assert(condition)                                                                                                                                    \
	do                                                                                                                                                              \
	{                                                                                                                                                               \
	    auto value = condition;                                                                                                                                     \
	    if (!value)                                                                                                                                                 \
	    {                                                                                                                                                           \
		Hermes::logerror(std::format("{}: \033[38;5;9mfailed_assertion\033[m: Hermes_AssertErr(\"{}\")\n", Hermes::current_source_location_str(), #condition)); \
		std::abort();                                                                                                                                           \
	    }                                                                                                                                                           \
	}                                                                                                                                                               \
	while (0)

#    define Hermes_AssertM(condition, message)                                                                                                                                       \
	do                                                                                                                                                                           \
	{                                                                                                                                                                            \
	    auto value = condition;                                                                                                                                                  \
	    if (!value)                                                                                                                                                              \
	    {                                                                                                                                                                        \
		Hermes::logerror(std::format("{}: \033[38;5;9mfailed_assertion\033[m: Hermes_AssertErr(\"{}\", {})\n", Hermes::current_source_location_str(), #condition, message)); \
		std::abort();                                                                                                                                                        \
	    }                                                                                                                                                                        \
	}                                                                                                                                                                            \
	while (0)

#else

#    define Hermes_DebugLog(message) ((void)0)
#    define Hermes_DebugLogLn(message) ((void)0)
#    define Hermes_DebugLogErr(message) ((void)0)
#    define Hermes_DebugLogWarn(message) ((void)0)
#    define Hermes_DebugLogNote(message) ((void)0)
#    define Hermes_Assert(condition) ((void)0)
#    define Hermes_AssertM(condition, message) ((void)0)

#endif