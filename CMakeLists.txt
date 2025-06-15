# CMake version
cmake_minimum_required(VERSION 3.15) 
# details about the project
project(Hermes
    VERSION 0.1.0
    LANGUAGES CXX
)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}")
set(VENDOR_DIR ${PROJECT_DIR}/Vendor)
set(SRC_DIR ${PROJECT_DIR}/Source)
# resources
set(RUNTIME_RESOURCES_DIR "${BIN_DIR}/Resources")
set(SOURCE_RESOURCES_DIR "${PROJECT_DIR}/Resources")

# where executables should go
# note: Allows SDL DLLS to be automatically placed in bin directory  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/$<CONFIGURATION>")
# cpp version = c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
# sdl
add_subdirectory(${VENDOR_DIR}/SDL EXCLUDE_FROM_ALL) # add SDL source
add_subdirectory(${VENDOR_DIR}/SDL_image EXCLUDE_FROM_ALL) # add SDL_Image source


# set(HEADER_FILES
#     "${SRC_DIR}/app.h"
#     "${SRC_DIR}/error.h"
#     "${SRC_DIR}/pch.h"
#     "${SRC_DIR}/surface.h"
#     "${SRC_DIR}/system.h"
#     "${SRC_DIR}/tray.h"
# )

# source files
set(SRC_FILES 
    "${SRC_DIR}/app.cpp"
    "${SRC_DIR}/error.cpp"
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/system.cpp"
    "${SRC_DIR}/tray.cpp"
)

set(PCH "${SRC_DIR}/pch.h")

# compile options
set(COMPILE_OPTS
    "-Wall"
    "-Wextra"
    "-Wno-unused"
)

# executable
set(TARGET_NAME hermes)
add_executable(${TARGET_NAME} ${SRC_FILES})
target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_OPTS})
target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH})
target_link_libraries(${TARGET_NAME} PRIVATE SDL3_image::SDL3_image SDL3::SDL3)


# add_custom_command(
#     TARGET ${TARGET_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#         "${SOURCE_RESOURCES_DIR}/icon128.png"
#         "${RUNTIME_RESOURCES_DIR}/icon128.png"
# )
                
file(COPY "${SOURCE_RESOURCES_DIR}/icon128.png" DESTINATION "${RUNTIME_RESOURCES_DIR}")