# CMake version
cmake_minimum_required(VERSION 3.20) 
# details about the project
project(Hermes
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_DEBUG_POSTFIX d)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE is undefined. Setting to \"Debug\"")
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(PROJECT_DIR             "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
set(BIN_DIR                 "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")
set(VENDOR_DIR              "${PROJECT_DIR}/Vendor")
set(SRC_DIR                 "${PROJECT_DIR}/Source")
set(SOURCE_RESOURCES_DIR    "${PROJECT_DIR}/Resources")
set(RUNTIME_RESOURCES_DIR   "${BIN_DIR}")

# where executables should go
# note: Allows SDL DLLS to be automatically placed in bin directory  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# cpp version = c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
# sdl
add_subdirectory(${VENDOR_DIR}/SDL EXCLUDE_FROM_ALL) # add SDL source
add_subdirectory(${VENDOR_DIR}/SDL_image EXCLUDE_FROM_ALL) # add SDL_Image source

# source files
set(SRC_FILES 
    "${SRC_DIR}/app.cpp"
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/system.cpp"
    "${SRC_DIR}/SDL/init.cpp"
    "${SRC_DIR}/SDL/misc.cpp"
    "${SRC_DIR}/SDL/surface.cpp"
    "${SRC_DIR}/SDL/tray.cpp"
)

set(PCH "${SRC_DIR}/pch.h")

# compile options
set(OPTIONS
    "-Wall"
    "-Wextra"
    "-Wno-unused"
)

# executable
set(TARGET hermes)
add_executable(${TARGET} ${SRC_FILES})
target_precompile_headers(${TARGET} PRIVATE ${PCH})
target_link_libraries(
    ${TARGET} PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image 
)
set_target_properties(
    ${TARGET} 
    PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_compile_options(
    ${TARGET} PRIVATE
    "-Wall"
    "-Wextra"
    "-Wno-unused"
)

# Config specific settings
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_options(
        ${TARGET} PRIVATE 
        "-g"
    )
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    target_compile_options(
        ${TARGET} PRIVATE 
        "-O3"
    )
    target_compile_definitions(
        ${TARGET} PRIVATE 
        "NDEBUG"
    )
endif()

function(copy_resource name)
    add_custom_command(
        TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            "${SOURCE_RESOURCES_DIR}/${name}"
            "${RUNTIME_RESOURCES_DIR}/${name}"
        COMMENT "Copying file"
    )
endfunction()

copy_resource("icon128.png")