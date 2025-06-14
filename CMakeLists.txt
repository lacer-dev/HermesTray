# CMake version
cmake_minimum_required(VERSION 3.15) 
# details about the project
project(afk_window
    VERSION 0.0.1
    LANGUAGES CXX
)

# where executables should go
# note: Allows SDL DLLS to be automatically placed in bin directory  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>")
# cpp version = c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL) # add SDL source
add_subdirectory(vendor/SDL_image EXCLUDE_FROM_ALL) # add SDL_Image source
# source files
set(SRC_FILES 
    "src/app.h"
    "src/error.h"
    "src/main.cpp"
    "src/surface.h"
    "src/timing.h"
    "src/tray.cpp"
    "src/tray.h"
)

set(COMPILE_OPTS
    "-Wall"
    "-Wextra"
    "-Wno-unused"
)

# executable
add_executable(afkwindow ${SRC_FILES})
target_compile_options(afkwindow PUBLIC ${COMPILE_OPTS})
target_link_libraries(afkwindow PRIVATE SDL3_image::SDL3_image SDL3::SDL3)