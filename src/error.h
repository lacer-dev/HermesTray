#pragma once
#include <exception>
#include <format>
#include <print>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>

#include "sys.h"

template<class... Args>
inline void eprint(std::format_string<Args...> fmt, Args&&... args) {
	std::print(stderr, fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void eprintln(std::format_string<Args...> fmt, Args&&... args) {
	eprint("{}\n", std::format(fmt, std::forward<Args>(args)...));
}

inline void eprintln() { eprintln(""); }

namespace hermes {
	template<class... Args>
	inline void error(std::format_string<Args...> fmt, Args&&... args) {
		eprintln("\033[38;5;1merror\033[m: {}", std::format(fmt, std::forward<Args>(args)...));
	}
	
	template<class... Args>
	[[noreturn]] inline void fatal(std::format_string<Args...> fmt, Args&&... args) {
		eprintln("\033[38;5;160mfatal error\033[m: {}", std::format(fmt, std::forward<Args>(args)...));
		std::terminate();
	}

	inline void
		_force_assert(std::string_view _message, std::source_location _location = std::source_location::current());

	inline bool _validate_condition(
		bool								condition,
		[[maybe_unused]] const std::string& success_message = " done\n",
		[[maybe_unused]] const std::string& fail_message	= " failed\n") noexcept;

	void _force_assert(std::string_view _message, std::source_location _location) {
		eprintln(
			"\033[38;5;9mfailed assertion\033[m at {}:{}:{} in function '{}': {}",
			_location.file_name(),
			_location.line(),
			_location.column(),
			_location.function_name(),
			_message);
		std::terminate();
	}

	bool _validate_condition(bool condition, const std::string& true_msg, const std::string& false_msg) noexcept {
#ifndef NDEBUG
		if (condition) {
			if (!true_msg.empty()) {
				eprint("{}", true_msg);
			}
		} else {
			if (!false_msg.empty()) {
				eprint("{}", false_msg);
			}
		}
#endif
		return condition;
	}
}; // namespace hermes

// Debug Macros
////////////////////////////////////////////////////////////
// `dbg_validate(bool condition, const std::string& true_msg, const std::string& false_msg)`
// If `condition` is true, prints `true_msg`, otherwise prints `false_msg`
//
// `template<class... Args>
// void dbg(std::format_string<Args...> fmt, Args&&... args)`
//
//
// `bool dbg_assert(bool condition, const std::string& success_message = " done\n",
//     const std::string& fail_message	= " failed\n") noexcept`
////////////////////////////////////////////////////////////

#define dbg_validate(...) ::hermes::_validate_condition(__VA_ARGS__)

#ifndef NDEBUG
	#define dbg(...) 		do { eprint(__VA_ARGS__); } while (0)
	#define dbg_assert(...)	do { if (!(__VA_ARGS__)) ::hermes::_force_assert(#__VA_ARGS__); } while (0)
#else
	#define dbg(...) 		((void)0)
	#define dbg_assert(...)	((void)0)
#endif