#pragma once
#include <cassert>
#include <exception>
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
		error,
		warning,
	};

	inline const std::string_view to_string(logl level) {
		switch (level) {
		case logl::error:
			return "\033[38;5;1merror\033[m";
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
	[[noreturn]]
	inline void fatal_error(std::format_string<Args...> fmt, Args&&... args) {
		eprintln("\033[38;5;160mfatal error\033[m: {}", std::format(fmt, std::forward<Args>(args)...));
		std::terminate();
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
		std::terminate();
	}

	inline bool _validate_condition(
		bool condition,
		[[maybe_unused]] std::string success_message = " done\n",
		[[maybe_unused]] std::string fail_message = " failed\n") {
#ifndef NDEBUG
		if (condition) {
			if (!success_message.empty()) {
				eprint("{}", success_message);
			}
		} else {
			if (!fail_message.empty()) {
				eprint("{}", fail_message);
			}
		}
#endif
		return condition;
	}
}; // namespace hermes

// Debug macros
#ifndef NDEBUG
	#define dbgdo(...)	 	do { __VA_ARGS__; } while (0)
	#define dbg(...) do { eprint(__VA_ARGS__); } while (0)
	#define dbgassert(...)	do { if (!(__VA_ARGS__)) ::hermes::_force_assert(#__VA_ARGS__); } while (0)
	#define dbgvalidate(...) ::hermes::_validate_condition(__VA_ARGS__)
#else
	#define dbgdo(...)		((void)0)
	#define dbg(...) 	((void)0)
	#define dbgassert(...)	((void)0)
	#define dbgvalidate(...) ::hermes::_validate_condition(__VA_ARGS__)
#endif