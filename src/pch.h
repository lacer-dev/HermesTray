#pragma once

#include <cassert>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <nameof/nameof.hpp>

#if defined(_WIN32)
	#include <windows.h>
#elif defined(__unix__)
	#include <unistd.h>
#else
	#error "This platform is not supported (Only Windows is supported)"
#endif