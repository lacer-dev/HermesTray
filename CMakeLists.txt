cmake_minimum_required(VERSION 3.15) 
project(Hermes LANGUAGES CXX)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR "error: this platform ${CMAKE_SYSTEM_NAME} is not supported.")
endif()

# build type

set(AVAILABLE_BUILD_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
elseif(NOT CMAKE_BUILD_TYPE IN_LIST AVAILABLE_BUILD_TYPES)
    message(FATAL_ERROR "error: \"${CMAKE_BUILD_TYPE}\" is not a supported value for CMAKE_BUILD_TYPE.")
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

# dependencies

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# variables

set(TARGET hermes)
set(SRC    ${CMAKE_SOURCE_DIR}/src)
set(RES    ${CMAKE_SOURCE_DIR}/resources)
set(BIN    ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_LOWERCASE})
set(PCH    ${SRC}/pch.h)
set(SOURCES
    ${SRC}/app.cpp
    ${SRC}/main.cpp
    ${SRC}/sys.cpp
    ${SRC}/window.cpp
    ${SRC}/window.h
    ${SRC}/systray.cpp
    $<$<PLATFORM_ID:Windows>:${SRC}/platform/win32/sys.cpp>
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configuration

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused)
add_link_options(-static-libstdc++ -static-libgcc)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_definitions(NDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Oz)
endif()

# create executable
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} SDL3::SDL3 SDL3_image::SDL3_image stdc++exp)
target_precompile_headers(${TARGET} PRIVATE ${PCH})

# post-build: copy resources
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E create_symlink "${RES}/hermes32.png" "${BIN}/hermes32.png"
)