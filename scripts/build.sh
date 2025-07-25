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
# brighten <0-23 | 'm'> <string>...
brighten() {
    local brightness=15
    local start_i=2
    [[ "$1" =~ ^[0-9]+$ && "$1" -ge 0 && "$1" -lt 23 ]] && brightness=$((232 + $1)) || start_i=1
    pcolor "$brightness" "${@:$start_i}"
}

SL=$(pcolor 98 "$(basename $0)")
SLC=$(pcolor 98 "$(basename $0): ")
EL=$(pcolor 1 "error")
ELC=$(pcolor 1 "error: ")

# pmsg <string>
pmsg() {
    printf -- "%s" "$SLC$(brighten "${@:1}")"
}
# perror <string>
perror() {
    printf -- "%s" "$SLC$ELC$(brighten "${@:1}")" >&2
}
# pnote <string>
pnote() {
    printf -- "%s" "$SLC$(pcolor 3 "note: ")$(brighten "${@:1}")" >&2
}
# line <fill-str> [begin-str]
line() {
    printf -- "%s" "$2"
    printf -- "$1%.0s" $(seq -s ' ' 0 "$(($(tput cols) - 1 - ${#2}))")
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
    echo "      --clean                Cleans the build directory."
    echo "  -c, --config <config>      Specifies the build configuration. Allowed values are (case-insensitive) 'debug', 'deb' (for Debug),"
    echo "                             'release', 'rel' (for Release), 'rwd', and 'relwithdebinfo' (for RelWithDebInfo). The default value"
    echo "                             is 'debug'"
    echo "  -s, --supress-build-output Supress standard output (but not standard error) during build."
    echo "  -h, --help                 Display this message and exit."
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
    cmake --build build
}
export -f build_target

clean() {
    rm -rf build/*;
}

trap "pmsg \"exiting build script...\"" EXIT

# run clean if '--clean' option is specified
if [[ $RUN_CLEAN == 'y' ]]; then
    pmsg "cleaning build directory...\n"
    [[ $SUPRESS_OUT == 'y' ]] && OUT=$(clean) || clean
    ec=$?
    if [[ 0 -ne $ec ]]; then
        perror "clean failed with cmake exit code $ec. here is the output:\n"
        echo "$OUT" >&2
        exit 2
    else
        pmsg "successfully cleaned build directory.\n"
        exit 0
    fi
fi

# otherwise run cmake and build
pmsg "running build script for '${CMAKE_BUILD_TYPE,,}'...\n"
pmsg "generating build files...\n"
mkdir -p build
OUT=$(generate_build 2>&1)
ec=$?
if [[ 0 -ne $ec ]]; then
    perror "generating build files failed with cmake exit code $ec.\n"
    pnote "output:\n"
    brighten 21 $(line ─ "──cmake")
    echo "$OUT" >&2
    brighten 21 $(line ─)
    exit 2
else
    pmsg "successfully generated build files.\n"
fi
##################################################
pmsg "building target '$TARGET'...\n"
if [[ $SUPRESS_OUT == 'n' ]]; then
    brighten 21 $(line ─ "──build")
    build_target
    brighten 21 $(line ─)
else
    OUT=$(script -e -q /dev/null -c "build_target" 2>&1)
fi
ec=$?
if [[ 0 -ne $ec ]]; then
    perror "build failed with cmake exit code $ec.\n"
    if [[ $SUPRESS_OUT == 'y' ]]; then
        pnote "output:\n"
        brighten 21 $(line ─ "──build")
        echo "$OUT" >&2
        brighten 21 $(line ─)
    fi
    exit 1
else
    pmsg "successfully built target '$TARGET'.\n"
    pmsg "build was output written to \"./bin/${CMAKE_BUILD_TYPE,,}\".\n"
fi
##################################################
exit 0