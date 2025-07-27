#pragma once
#include <cassert>
#include <format>
#include <print>
#include <source_location>
#include <string>

#include "sys.h"

template<class... Args>
inline void eprint(std::format_string<Args...> fmt, Args&&... args)
{
	std::print(stderr, fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void eprintln(std::format_string<Args...> fmt, Args&&... args)
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

inline constexpr std::string colorstr(Ansi color, const std::string& str)
{
	return std::format("\033[38;5;{}m{}\033]m", static_cast<unsigned>(color),
					   str);
}

inline constexpr std::string colorstr(unsigned ansi_color_code,
									  const std::string& str)
{
	return std::format("\033[38;5;{}m{}\033]m", ansi_color_code, str);
}

namespace prefixes {

const inline std::string SDL = colorstr(6, "SDL");
const inline std::string ERR = colorstr(1, "error");
const inline std::string NOTE = colorstr(14, "note");
const inline std::string WARN = colorstr(227, "warning");

} // namespace prefixes

template<class... Args>
inline void printerror(std::format_string<Args...> fmt, Args&&... args)
{
	eprintln("{}: {}", prefixes::ERR,
			 std::format(fmt, std::forward<Args>(args)...));
}

namespace hermes {

///// Error Generators /////

constexpr std::string messagebox_title_error()
{
	return process::procname() + " - Error";
}

constexpr std::string messagebox_title_warning()
{
	return process::procname() + " - Warning";
}

constexpr std::string messagebox_title_info()
{
	return process::procname() + " - Information";
}

// constexpr std::string directory_not_found(const std::string& directory)
// {
//     return std::format("directory not found: could not find directory at
//     \"{}\"", directory);
// }

// constexpr std::string file_not_found(const std::string& file)
// {
//     return std::format("file not found: could not find file at \"{}\"",
//     file);
// }

inline void _hermes_assert_internal(
	std::string_view _message,
	std::source_location _location = std::source_location::current())
{
	eprintln("{}: {}: at {}:{}:{} in function \"{}\": "
			 "hermes_assert(\"{}\")\n",
			 colorstr(1, "error:"), colorstr(9, "failed assertion"),
			 _location.file_name(), _location.line(), _location.column(),
			 _location.function_name(), _message);
	std::abort();
}

}; // namespace hermes

#ifndef NDEBUG

#define hermes_dbgprint(message) \
	do                           \
	{                            \
		eprint(message);         \
	}                            \
	while (0)
#define hermes_dbgprintln(message)    \
	do                                \
	{                                 \
		eprintln("{}", #message) << ; \
	}                                 \
	while (0)
#define hermes_assert(...)                                 \
	do                                                     \
	{                                                      \
		if (!(__VA_ARGS__))                                \
			hermes::_hermes_assert_internal(#__VA_ARGS__); \
	}                                                      \
	while (0)
#else
#define hermes_dbgprint(message) ((void)0)
#define hermes_dbgprintln(message) ((void)0)
#define hermes_assert(message) ((void)0)
#endif