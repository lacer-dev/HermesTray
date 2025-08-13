cmake_minimum_required(VERSION 3.15) 
project(Hermes LANGUAGES CXX)

# build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
elseif(NOT CMAKE_BUILD_TYPE MATCHES "Debug|Release|RelWithDebInfo|MinSizeRel")
    message(FATAL_ERROR "error: \"${CMAKE_BUILD_TYPE}\" is not a supported value for CMAKE_BUILD_TYPE.")
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

# output location
if(NOT HERMES_OUTPUT_DIR)
    set(HERMES_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_LOWERCASE})
endif()

# dependencies
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# variables
set(TARGET hermes)
set(SRC    ${CMAKE_SOURCE_DIR}/src)
set(RES    ${CMAKE_SOURCE_DIR}/resources)
set(BIN    ${HERMES_OUTPUT_DIR})
set(VENDOR ${CMAKE_SOURCE_DIR}/vendor)
set(PCH    ${SRC}/pch.h)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

# create executable
add_executable(${TARGET} 
    ${SRC}/main.cpp
    ${SRC}/sys.cpp
    $<$<PLATFORM_ID:Windows>:${SRC}/platform/windows/windows_sys.cpp>
    $<$<PLATFORM_ID:Linux>:${SRC}/platform/posix/posix_sys.cpp>
)
target_link_libraries(${TARGET} SDL3::SDL3 SDL3_image::SDL3_image stdc++exp)
target_precompile_headers(${TARGET} PRIVATE ${PCH})
target_include_directories(${TARGET} PRIVATE ${VENDOR}/nameof/include)

# configuration
target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused)
target_link_options(${TARGET} PRIVATE -static-libstdc++ -static-libgcc)

if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
    target_compile_options(${TARGET} PRIVATE -g)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    add_compile_definitions(NDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TARGET} PRIVATE -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${TARGET} PRIVATE -O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_options(${TARGET} PRIVATE -O2)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    target_compile_options(${TARGET} PRIVATE -Oz)
endif()

# post-build: copy resources
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E create_symlink "${RES}/hermes32.png" "${BIN}/hermes32.png"
)