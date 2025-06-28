include("Hermes.cmake")

# CMake version
cmake_minimum_required(VERSION 3.20) 

set(CMAKE_DEBUG_POSTFIX d)

# Set BUILD_TYPE
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
hermes_set_build_type("${CMAKE_BUILD_TYPE}")
message("Hermes: CMAKE_BUILD_TYPE = \"${CMAKE_BUILD_TYPE}\"")

hermes_set_compiler("${CMAKE_CXX_COMPILER_ID}")
# message("Hermes: CMAKE_CXX_COMPILER_ID = \"${CMAKE_CXX_COMPILER_ID}\"")

# PROJECT
project(Hermes
    VERSION 0.1.0
    LANGUAGES CXX
)

# C++ version = c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TARGET                  hermes)
set(PROJECT_DIR             "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
set(BIN_DIR                 "${CMAKE_SOURCE_DIR}/bin/${HERMES_BUILD_TYPE}/${PROJECT_NAME}")
set(VENDOR_DIR              "${PROJECT_DIR}/Vendor")
set(SRC_DIR                 "${PROJECT_DIR}/Source")
set(SRC_RESOURCE_DIR        "${PROJECT_DIR}/Resources")
set(OUT_RESOURCE_DIR        "${BIN_DIR}")
set(PCH                     "${SRC_DIR}/pch.h")
# note: Allows SDL DLLS to be automatically placed in bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# Dependencies
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
add_subdirectory(${VENDOR_DIR}/SDL EXCLUDE_FROM_ALL) # add SDL source
add_subdirectory(${VENDOR_DIR}/SDL_image EXCLUDE_FROM_ALL) # add SDL_Image source

if(HERMES_COMPILER_MSVC)
    # All
    set(OPTIONS         /W4)
    set(DEFINES)
    # Debug
    set(OPTIONS_DEBUG   /DEBUG)
    set(DEFINES_DEBUG   DEBUG)
    # Release
    set(OPTIONS_RELEASE /DEBUG /O2)
    set(DEFINES_RELEASE NDEBUG)
    # Dist
    set(OPTIONS_DIST    /O2)
    set(DEFINES_DIST    NDEBUG)
else()
    # All
    set(OPTIONS         -Wall -Wextra -Wno-unused)
    set(DEFINES)
    # Debug
    set(OPTIONS_DEBUG   -g)
    set(DEFINES_DEBUG)
    # Release
    set(OPTIONS_RELEASE -g -O3)
    set(DEFINES_RELEASE NDEBUG)
    # Dist
    set(OPTIONS_DIST    -O3)
    set(DEFINES_DIST    )
endif()

set(SRC_FILES
    ${SRC_DIR}/windows/windows_system.cpp
    ${SRC_DIR}/app.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/system.cpp
    ${SRC_DIR}/systray.cpp
)

# executable
add_executable(${TARGET} ${SRC_FILES})
target_compile_options(${TARGET} PRIVATE ${OPTIONS})
target_link_libraries(${TARGET} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
target_precompile_headers(${TARGET} PRIVATE ${PCH})
# debug postfix
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})


# Config specific settings
if(HERMES_CONFIG_DEBUG)
    target_compile_options(${TARGET}        PRIVATE ${DEFINES_DEBUG})
    target_compile_definitions(${TARGET}    PRIVATE ${DEFINES_DEBUG})
elseif(HERMES_CONFIG_RELEASE)
    target_compile_options(${TARGET}        PRIVATE ${DEFINES_RELEASE})
    target_compile_definitions(${TARGET}    PRIVATE ${DEFINES_RELEASE})
elseif(HERMES_CONFIG_DIST)
    target_compile_options(${TARGET}        PRIVATE ${DEFINES_DIST})
    target_compile_definitions(${TARGET}    PRIVATE ${DEFINES_DIST})
endif()

hermes_copy_resource_post_build("hermes32.png")