#pragma once
#include <cassert>
#include <format>
#include <print>
#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

#include "sys.h"

// Logging

template<class... Args>
inline void eprint(std::format_string<Args...> fmt, Args&&... args) {
	std::print(stderr, fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void eprintln(std::format_string<Args...> fmt, Args&&... args) {
	eprint("{}\n", std::format(fmt, std::forward<Args>(args)...));
}

template<class... Args>
inline void eprintln() {
	eprint("\n");
}

namespace hermes {
	enum class logl {
		fatal,
		error,
		warning,
		note,
	};

	inline const std::string_view to_string(logl level) {
		switch (level) {
		case logl::fatal:
			return "\033[38;5;m160fatal error\033[m";
		case logl::error:
			return "\033[38;5;1merror\033[m";
		case logl::note:
			return "\033[38;5;14mnote\033[m";
		case logl::warning:
			return "\033[38;5;227mwarning\033[m";
		default:
			throw std::length_error("invalid log level");
		}
	}

	// an alias to eprintln
	template<class... Args>
	inline void log(std::format_string<Args...> fmt, Args&&... args) {
		eprintln("{}", std::format(fmt, std::forward<Args>(args)...));
	}

	template<class... Args>
	inline void log(logl level, std::format_string<Args...> fmt, Args&&... args) {
		log("{}: {}", to_string(level), std::format(fmt, std::forward<Args>(args)...));
	}

	template<class... Args>
	inline void logerror(std::format_string<Args...> fmt, Args&&... args) {
		log(logl::error, fmt, std::forward<Args>(args)...);
	}

	template<class... Args>
	inline void lognote(std::format_string<Args...> fmt, Args&&... args) {
		log(logl::note, fmt, std::forward<Args>(args)...);
	}

	inline void
		_force_assert(std::string_view _message, std::source_location _location = std::source_location::current()) {
		eprintln(
			"\033[38;5;9mfailed assertion\033[m at {}:{}:{} in function \"{}\": {}",
			_location.file_name(),
			_location.line(),
			_location.column(),
			_location.function_name(),
			_message);
		std::abort();
	}
}; // namespace hermes

// Debug macros
#ifndef NDEBUG
	#define dbgdo(...)	 	do { __VA_ARGS__; } while (0)
	#define dbgprint(...) 	do { eprint(__VA_ARGS__); } while (0)
	#define dbgprintln(...) do { eprintln(__VA_ARGS__); } while (0)
	#define dbglog(...)	 	do { ::hermes::log(__VA_ARGS__); } while (0)
	#define dbgassert(...)	do { if (!(__VA_ARGS__)) ::hermes::_force_assert(#__VA_ARGS__); } while (0)
#else
	#define dbgdo(...)		((void)0)
	#define dbgprint(...) 	((void)0)
	#define dbgprintln(...)	((void)0)
	#define dbglog(...)		((void)0)
	#define dbgassert(...)	((void)0)
#endif