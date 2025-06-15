#pragma once

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_tray.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>

#ifdef _WIN32
#include <windows.h>
#else
#error "This platform is not supported"
#endif