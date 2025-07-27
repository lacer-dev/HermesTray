#!/usr/bin/env bash

# line <fill-str> [begin-str]
line() {
    printf -- "%s" "$2"
    printf -- "$1%.0s" $(seq -s ' ' 0 "$(($(tput cols) - 1 - ${#2}))")
}
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

# build_pmsg <string>
build_pmsg() {
    printf -- "%s" "$SLC$(brighten "${@:1}")"
}
# build_perror <string>
build_perror() {
    printf -- "%s" "$SLC$ELC$(brighten "${@:1}")" >&2
}
# build_pnote <string>
build_pnote() {
    printf -- "%s" "$SLC$(pcolor 3 "note: ")$(brighten "${@:1}")" >&2
}

translate_build_type() {
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
    (msr | minsizerel)
        printf "MinSizeRel"
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
    echo "                             'release', 'rel' (for Release), 'rwd', and 'relwithdebinfo' (for RelWithDebInfo). If not specified, the"
    echo "                             default value is 'debug'"
    echo "  -j, --parallel <jobs>      Builds in parallel with <jobs> number of jobs. If not specified, the default value is nproc + 1."
    echo "  -s, --supress-build-output Supress standard output (but not standard error) during build."
    echo "  -h, --help                 Display this message and exit."
    echo
    echo "Exit Codes:"
    echo "  0 if build was successful,"
    echo "  1 if there was an error while building,"
    echo "  2 if there was an error otherwise."
}

build_exit() {
    build_pmsg "exiting script with exit code $1... \n"
    exit $1
}
##################################################################################################################
# parse command-line options
OPTIONS=$(getopt -n "$SLC$EL$(brighten)" -o "hc:j:s" -l "help,clean,config:,parallel:suppress-build-output," -- "$@")
ec=$?
if [[ $ec -eq 0 ]]; then
    eval set -- $OPTIONS
else
    if [[ $ec -eq 1  ]]; then 
        pcolor
    else
        build_perror "getopt failed with exit code $ec."
    fi
    build_exit 2
fi

CMAKE_BUILD_TYPE=Debug
NUM_JOBS=$(( $(nproc) + 1 ))
RUN_CLEAN='n'
SUPRESS_OUT='n'
while [ $# -gt 0 ]; do
    case $1 in
    (-h | --help)
        printhelp
        exit 0
        ;;
    (--clean)
        RUN_CLEAN='y'
        ;;
    (-j | --jobs)
        NUM_JOBS=$2
        shift
        ;;
    (-s | --suppress-build-output)
        SUPRESS_OUT='y'
        ;;
    (-c | --config)
        CMAKE_BUILD_TYPE=$(translate_build_type $2)
        if [[ $? -ne 0 ]]; then
            build_perror "unknown build configuration '$2'\n" >&2
            build_exit 2
        fi
        shift
        ;;
    (--)
        shift
        break
        ;;
    (-*)
        build_perror "unrecognized option '$1'"
        build_exit 2
        ;;
    (*)
        break
        ;;
    esac
    shift
done
# there should be no more arguments
if [ $# -ne 0 ]; then
    for arg in $@; do
        build_perror "invalid argument '$arg'.\n"
    done
    build_exit 2
fi

##################################################################################################################
export TARGET="hermes"

build_generate() {
    cmake -S . -B build -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE;
}

build_target() {
    cmake --build build -t $TARGET -j $NUM_JOBS
}
export -f build_target

build_clean() {
    rm -rf build/*;
}

# run clean if '--clean' option is specified
if [[ $RUN_CLEAN == 'y' ]]; then
    build_pmsg "cleaning build directory...\n"
    [[ $SUPRESS_OUT == 'y' ]] && OUT=$(build_clean) || build_clean
    ec=$?
    if [[ 0 -ne $ec ]]; then
        build_perror "clean failed (with cmake exit code $ec). here is the output:\n"
        echo "$OUT" >&2
        build_exit 2
    else
        build_pmsg "successfully cleaned build directory.\n"
        build_exit 0
    fi
fi

# otherwise run cmake and build
build_pmsg "running build script for '${CMAKE_BUILD_TYPE,,}'...\n"
build_pmsg "generating build files...\n"
mkdir -p build
OUT=$(build_generate 2>&1)
ec=$?
if [[ 0 -ne $ec ]]; then
    build_perror "generating build files failed (with cmake exit code $ec).\n"
    build_pnote "output:\n"
    brighten 21 $(line ─ "──cmake")
    echo "$OUT" >&2
    brighten 21 $(line ─)
    build_exit 2
else
    build_pmsg "successfully generated build files.\n"
fi
##################################################
build_pmsg "building target '$TARGET'...\n"
if [[ $SUPRESS_OUT == 'n' ]]; then
    brighten 21 $(line ─ "──build")
    build_target
    ec=$?
    brighten 21 $(line ─)
else
    # OUT=$(script -e -q /dev/null -c "build_target" 2>&1)
    OUT=$(build_target 2>&1)
    ec=$?
fi
if [[ 0 -ne $ec ]]; then
    build_perror "build failed (with cmake exit code $ec).\n"
    if [[ $SUPRESS_OUT == 'y' ]]; then
        build_pnote "output:\n"
        brighten 21 $(line ─ "──build")
        echo "$OUT" >&2
        brighten 21 $(line ─)
    fi
    build_exit 1
else
    # e
    build_pmsg "successfully built target '$TARGET'.\n"
    build_pmsg "build output was written to \"./bin/${CMAKE_BUILD_TYPE,,}\".\n"
    build_exit 0
fi
##################################################
