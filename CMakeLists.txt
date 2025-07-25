cmake_minimum_required(VERSION 3.20) 
project(Hermes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" 
    AND NOT CMAKE_BUILD_TYPE STREQUAL "Release" 
    AND NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
)
    message(FATAL_ERROR "error: \"${CMAKE_BUILD_TYPE}\" is not a valid value for CMAKE_BUILD_TYPE")
endif()

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

set(TARGET      hermes)
set(BIN_DIR     ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_LOWERCASE})
set(SRC_DIR     ${CMAKE_SOURCE_DIR}/src)
set(RES_DIR     ${CMAKE_SOURCE_DIR}/resources)
set(PCH         ${SRC_DIR}/pch.h)
set(SOURCES
    ${SRC_DIR}/app.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/sys.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/window.h
)

list(APPEND SOURCES $<$<PLATFORM_ID:Windows>:${SRC_DIR}/sys/win32/screensaver.cpp>)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})

add_compile_options(-static-libstdc++ -static-libgcc)
link_libraries(stdc++)
add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O3)
    add_compile_definitions(NDEBUG)
endif()

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} SDL3::SDL3 SDL3_image::SDL3_image stdc++exp)
target_precompile_headers(${TARGET} PRIVATE ${PCH})

# copy_resource_post_build("hermes32.png" ${BIN_DIR})

add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${RES_DIR}/hermes32.png" "${BIN_DIR}"
)