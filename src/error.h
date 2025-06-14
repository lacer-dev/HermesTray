#pragma once
#include <string>
#include <iostream>
#include <source_location>

inline std::string GetSourcePos(std::source_location location = std::source_location::current()) {
    return 
        std::string(location.file_name())
        + ':'
        + std::to_string(location.line()) 
        + ':' 
        + std::to_string(location.column());
}

inline std::string MakeErr(const std::string& message, bool sourcepos = true, std::source_location location = std::source_location::current()) {
    std::string str;
    if (sourcepos) {
        str += GetSourcePos(location);
        str += ": ";
    }
    str += "\033[31merror: \033[m" + message;
    return str;
}

inline void PrintErr(const std::string& message) {
    std::cerr << MakeErr(message);
}

#ifndef NDEBUG
#include <format>
#include <cstdlib>
#define DebugPrintErr(format_s, ...) do { std::cerr << std::format(format_s __VA_OPT__(,) __VA_ARGS__); } while (0)
#define DebugPrint(format_s, ...) do { std::cout << std::format(format_s __VA_OPT__(,) __VA_ARGS__); } while (0)
#define Assert(condition) do { \
    if (!condition) { \
        std::cerr << std::format("Assert Failed: Assert({})\n", #condition); \
        std::abort(); \
    } \
} while (0)
#define AssertErr(condition, message) do { \
    if (!condition) { \
        std::cerr << std::format("Assert Failed: Assert({})\n", #condition); \
        std::cerr << std::format("note: message: \"{}\"", message) << '\n'; \
        std::abort(); \
    } \
} while (0)
#else
#define DebugPrintErr(format_s, ...) ((void)0)
#define DebugPrint(format_s, ...) ((void)0)
#define Assert(condition) ((void)0)
#define AssertErr(condition, message) ((void)0)
#endif