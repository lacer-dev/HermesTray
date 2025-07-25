#!/usr/bin/env bash

# pcolor
# pcolor <color> [string]
pcolor() {
    if [[ $# -eq 0 ]]; then
        printf "\033[m"
    elif [[ $# -eq 1 ]]; then
        printf "\033[38;5;$1m"
    else
        printf "\033[38;5;$1m${@:2}\033[m"
    fi
}
# pnote <string>
brighten () {
    printf -- "%s" "$(pcolor 15 "$@")"
    # printf "%s" "$SLC$(pcolor 3 "note: ")$(pcolor 15 "${@:1}")" >&2
}

SL=$(pcolor 98 "$(basename $0)")
SLC=$(pcolor 98 "$(basename $0): ")
EL=$(pcolor 1 "error")
ELC=$(pcolor 1 "error: ")

# message <string>
message () {
    printf -- "%s" "$SLC$(brighten "${@:1}")"
}
# perror <string>
perror () {
    printf -- "%s" "$SLC$ELC$(brighten "${@:1}")" >&2
}
# pnote <string>
pnote () {
    printf -- "%s" "$SLC$(pcolor 3 "note: ")$(brighten "${@:1}")" >&2
}
# repeats <n> <string>
repeats () {
    printf -- "$2%.0s" $(seq -s ' ' 0 $1)
}
# line <fill-str> [begin-str]
line () {
    printf -- "%s" "$2"
    repeats "$(($(tput cols) - 1 - ${#2}))" $1
}

config2cmake() {
    case "${1,,}" in
    (debug | deb)
        printf "Debug"
        ;;
    (release | rel)
        printf "Release"
        ;;
    (rwd | relwithdebinfo)
        printf "RelWithDebInfo"
        ;;
    (*) 
        return 1
    esac

    return 0
}

printhelp() {
    echo "Usage: $(basename $0) [options]"
    echo
    echo "Options:"
    echo "      --clean                     Cleans the build directory."
    echo "  -c, --config <config>           Specifies the build configuration. Allowed values are (case-insensitive) 'debug', 'deb' (for Debug),"
    echo "                                  'release', 'rel' (for Release), 'rwd', and 'relwithdebinfo' (for RelWithDebInfo) Default value is 'debug'"
    echo "  -s, --supress-build-output      Supress standard output (but not standard error) during build."
    echo "  -h, --help                      Display this message and exit."
    echo
    echo "Exit Codes:"
    echo "  0 if build was successful,"
    echo "  1 if there was an error while building,"
    echo "  2 if there was an error otherwise."
}

##################################################################################################################
# parse command-line options
OPTIONS=$(getopt -n "$SLC$EL$(brighten)" -o "hc:s" -l "help,clean,config:,suppress-build-output," -- "$@")
ec=$?
if [ $ec -eq 1  ]; then
    pcolor
    exit 2
elif [ $ec -gt 1 ]; then
    exit 2
fi

eval set -- $OPTIONS

RUN_CLEAN='n'
SUPRESS_OUT='n'
CMAKE_BUILD_TYPE=Debug
while [ $# -gt 0 ]; do
    case $1 in
    (-h | --help)
        printhelp
        exit 0
        ;;
    (--clean)
        RUN_CLEAN='y'
        ;;
    (-s | --suppress-build-output)
        SUPRESS_OUT='y'
        ;;
    (-c | --config)
        CMAKE_BUILD_TYPE=$(config2cmake $2)
        if [[ $? -ne 0 ]]; then
            perror "unknown build configuration '$2'\n" >&2
            exit 2
        fi
        shift
        ;;
    (--)
        shift
        break
        ;;
    (-*)
        perror "unrecognized option '$1'"
        exit 2
        ;;
    (*)
        break
        ;;
    esac
    shift
done
# there should be no more arguments
if [ $# -ne 0 ]; then
    perror "invalid argument '$1'.\n"
    exit 2
fi

##################################################################################################################
TARGET="hermes"

generate_build() {
    cmake -S . -B build -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE;
}

build_target() {
    cmake --build build -j 9
}

clean() {
    rm -rf build/*;
}

trap "message \"exiting build script...\"" EXIT

# run clean if '--clean' option is specified
if [[ $RUN_CLEAN == 'y' ]]; then
    message "cleaning build directory...\n"
    [[ $SUPRESS_OUT == 'y' ]] && OUT=$(clean) || clean
    ec=$?
    if [[ 0 -ne $ec ]]; then
        perror "clean failed with cmake exit code $ec. here is the output:\n"
        echo "$OUT" >&2
        exit 2
    else
        message "successfully cleaned build directory.\n"
        exit 0
    fi
fi

# otherwise run cmake and build
message "running build script for '${CMAKE_BUILD_TYPE,,}'...\n"
message "generating build files...\n"
mkdir -p build
OUT=$(generate_build 2>&1)
ec=$?
if [[ 0 -ne $ec ]]; then
    perror "cmake failed with exit code $ec. here is the output:\n"
    pcolor $(line ─ "──cmake")
    echo "$OUT" >&2
    pcolor $(line ─)
    exit 2
else
    message "successfully generated build files.\n"
fi
##################################################
message "building target '$TARGET'...\n"
if [[ $SUPRESS_OUT == 'n' ]]; then
    pcolor $(line ─ "──build")
    build_target
    pcolor $(line ─)
else
    OUT=$(script -e -q /dev/null -c "build_target" 2>&1)
fi
ec=$?
if [[ 0 -ne $ec ]]; then
    perror "cmake --build failed with exit code $ec.\n"
    if [[ $SUPRESS_OUT == 'y' ]]; then
        pnote "output:\n"
        pcolor $(line ─ "──build")
        echo "$OUT" >&2
        pcolor $(line ─)
    fi
    exit 1
else
    message "successfully built target '$TARGET'.\n"
    message "build was output written to \"./bin/${CMAKE_BUILD_TYPE,,}\".\n"
fi
##################################################
exit 0