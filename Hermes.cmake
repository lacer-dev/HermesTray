# Set build type
function(hermes_set_build_type build_type)
    if(build_type STREQUAL "Debug")
        set(HERMES_BUILD_TYPE       "Debug" PARENT_SCOPE)
        set(HERMES_CONFIG_DEBUG     1       PARENT_SCOPE)
    elseif(build_type STREQUAL "RelWithDebInfo")
        set(HERMES_BUILD_TYPE       "RelWithDebInfo" PARENT_SCOPE)
        set(HERMES_CONFIG_RELEASE   1       PARENT_SCOPE)
    elseif(build_type STREQUAL "Release")
        set(HERMES_BUILD_TYPE       "Release"  PARENT_SCOPE)
        set(HERMES_CONFIG_DIST      1       PARENT_SCOPE)
    endif()
endfunction()

# Set compiler id
function(hermes_set_compiler compiler_id)
    if (compiler_id STREQUAL "GNU")
        set(HERMES_COMPILER         GNU     PARENT_SCOPE)
        set(HERMES_COMPILER_GNU     1       PARENT_SCOPE)
    elseif (compiler_id STREQUAL Clang)
        set(HERMES_COMPILER         "CLANG" PARENT_SCOPE)
        set(HERMES_COMPILER_CLANG   1       PARENT_SCOPE)
    elseif (compiler_id STREQUAL "MSVC")
        set(HERMES_COMPILER         MSVC    PARENT_SCOPE)
        set(HERMES_COMPILER_MSVC    1       PARENT_SCOPE)
    else()
        set(HERMES_COMPILER         UNKNOWN PARENT_SCOPE)
        set(HERMES_COMPILER_UNKNOWN 1       PARENT_SCOPE)
    endif()
endfunction()

# Copying resources
function(hermes_copy_resource_post_build name)
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SRC_RESOURCE_DIR}/${name}" "${OUT_RESOURCE_DIR}/${name}"
        COMMENT "Copying file")
endfunction()