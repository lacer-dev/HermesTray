#!/usr/bin/env bash

SLC=$(printf '\e[38;5;73m%s\e[m' "$(basename $0):")
ELC=$(printf '%s \e[38;5;1m%s\e[m' "${SLC}" 'error:')

# line [begin-str]
draw-line() {
  local ch='─'
  local text="${1//\*/${ch}}"
  local line_len="$(($(tput cols) - ${#text} - 1))"
  echo -ne "$text"
  printf "${ch}%.0s" $(seq -s ' ' 0 "$line_len")
  echo
}

get-build-type() {
  case "${1,,}" in
  (debug | deb)
    printf "Debug" ;;
  (release | rel)
    printf "Release" ;;
  (rwd | relwithdebinfo)
    printf "RelWithDebInfo" ;;
  (msr | minsizerel)
    printf "MinSizeRel" ;;
  (*) 
    if [[ $? -ne 0 ]]; then
      echo "${ELC} unknown build configuration '$2'" >&2
      exit 2
    fi
  esac
  return 0
}

print-help() {
  echo "Usage: $(basename $0) [options]"
  echo
  echo "Options:"
  echo "      --clean                Cleans the build directory."
  echo "  -c, --config <config>      Specifies the build configuration. Allowed values are"
  echo "                              (case-insensitively) 'debug|deb', 'release|rel',"
  echo "                              'relwithdebinfo|rwd', and 'minsizerel|msr'. If not"
  echo "                              specified, the default value is 'debug'"
  echo "  -j, --parallel <jobs>      Builds in parallel with <jobs> number of jobs. If not"
  echo "                              specified, the default value is nproc + 1."
  echo "      --cmake-options <options>"
  echo "                             Passes <options> to CMake before generating build files."
  echo "  -h, --print-help                 Display this message and exit."
  echo
  echo "Exit Codes:"
  echo "  0 if build was successful,"
  echo "  1 if there was an error while building,"
  echo "  2 if there was an error otherwise."
}

##################################################################################################################
# parse command-line options
OPTIONS=$(getopt -a -o "hc:j:s" -l "help,clean,config:,parallel:,cmake-options:" -- "$@")
[[ $? -eq 0 ]] && eval set -- $OPTIONS || exit 2

CMAKE_BUILD_TYPE=Debug
EXTRA_CMAKE_FLAGS=
NUM_JOBS=$(($(nproc) + 1))
TARGET="hermes"
while [ $# -gt 0 ]; do
  case $1 in
  (-h | --help)
    print-help
    exit 0
    ;;
  (--clean)
    TARGET='clean'
    ;;
  (-j | --jobs)
    NUM_JOBS=$2
    shift
    ;;
  (-c | --config)
    CMAKE_BUILD_TYPE=$(get-build-type $2)
    shift
    ;;
  (--cmake-options)
    EXTRA_CMAKE_FLAGS="$2"
    shift
    ;;
  (--)
    shift
    break
    ;;
  (-*)
    echo "${ELC} unrecognized option '$1'" >&2
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
  for arg in "$@"; do
    echo "${ELC} invalid argument '$arg'." >&2
  done
  exit 2
fi

##################################################################################################################

# run clean if '--clean' option is specified

if [[ "$TARGET" == clean ]]; then
  echo "${SLC} cleaning build directory..."
  cmake --build build --target clean >/dev/null 2>&1
  errc=$?
  if [[ $errc -ne 0 ]]; then
    echo "${ELC} cleaning build directory failed." >&2
    exit 1
  else
    echo "${SLC} successfully cleaned build directory."
  fi
  exit 0
fi

# otherwise run cmake and build

echo "${SLC} running build script for '${CMAKE_BUILD_TYPE,,}'..."
echo "${SLC} generating build files..."
draw-line '*CMake'
mkdir -p build
cmake \
  -S . \
  -B build \
  -G "Unix Makefiles" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
  $EXTRA_CMAKE_FLAGS
errc=$?
draw-line
if [[ 0 -ne $errc ]]; then
  echo "${ELC} generating build files failed (with cmake exit code $errc)." >&2
  exit 2
else
  echo "${SLC} successfully generated build files."
fi

echo "${SLC} building target '${TARGET}'..."
draw-line '*Build'
cmake \
  --build build \
  -t "${TARGET}" \
  -j "$NUM_JOBS"
errc=$?
draw-line

if [[ 0 -ne $errc ]]; then
  echo "${ELC} build failed (with cmake exit code $errc)." >&2
  exit 1
else
  echo "${SLC} successfully built target '${TARGET}'."
  exit 0
fi
##################################################
