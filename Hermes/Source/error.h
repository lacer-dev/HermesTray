#pragma once
#include <string>
#include <iostream>
#include <source_location>

namespace Hermes {

inline std::string source_location_to_string(std::source_location location = std::source_location::current());

inline std::string MakeFatal(const std::string& message);
inline std::string MakeError(const std::string& message);
inline std::string MakeWarning(const std::string& message);
inline std::string MakeNote(const std::string& message);
inline std::string MakeSDLError(const std::string& message);

inline std::ostream& LogFatal(const std::string& message);
inline std::ostream& LogError(const std::string& message);
inline std::ostream& LogWarn(const std::string& message);
inline std::ostream& LogNote(const std::string& message);
inline std::ostream& LogSDLError(const std::string& message);

std::string source_location_to_string(std::source_location location) {
    return std::string(location.file_name())
        + ':' + std::to_string(location.line()) 
        + ':' + std::to_string(location.column());
}

std::string MakeFatal(const std::string& message) {
    return "\033[38;5;196mfatal error:\033[m " + message;
}

std::string MakeError(const std::string& message) {
    return "\033[38;5;9merror:\033[m " + message;
}

std::string MakeWarning(const std::string& message) {
    return "\033[38;5;227mwarning:\033[m " + message;
}

std::string MakeNote(const std::string& message) {
    return "\033[38;5;14mnote:\033[m " + message;
}

std::string MakeSDLError(const std::string& message) {
    return "\033[38;5;6mSDL:\033[m " + message;
}

std::ostream& LogFatal(const std::string& message) {
    return std::cerr << MakeFatal(message);
}

std::ostream& LogError(const std::string& message) {
    return std::cerr << MakeError(message);
}

std::ostream& LogWarn(const std::string& message) {
    return std::cerr << MakeWarning(message);
}

std::ostream& LogNote(const std::string& message) {
    return std::cerr << MakeNote(message);
}

std::ostream& LogSDLError(const std::string& message) {
    return std::cerr << MakeSDLError(message);
}

};

#ifndef NDEBUG

#define Hermes_DebugLog(message) do { std::cerr << message; } while (0)
#define Hermes_DebugLogLn(message) do { std::cerr << message << '\n'; } while (0)
#define Hermes_DebugLogError(message) do { Hermes::LogError(message); } while (0)
#define Hermes_DebugLogWarn(message) do { Hermes::LogWarn(message); } while (0)
#define Hermes_DebugLogNote(message) do { Hermes::LogNote(message); } while (0)

#define Hermes_Assert(condition) do { \
    auto value = condition; \
    if (!value) { \
        Hermes::LogError(std::format("{}: \033[38;5;9mfailed_assertion\033[m: Hermes_AssertErr(\"{}\")\n", Hermes::source_location_to_string(), #condition)); \
        std::abort(); \
    } \
} while (0)

#define Hermes_AssertM(condition, message) do { \
    auto value = condition; \
    if (!value) { \
        Hermes::LogError(std::format("{}: \033[38;5;9mfailed_assertion\033[m: Hermes_AssertErr(\"{}\", {})\n", Hermes::source_location_to_string(), #condition, message)); \
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
#define Hermes_AssertM(condition, message) ((void)0)

#endif