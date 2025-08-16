#!/usr/bin/env bash

# line [begin-str]
function draw-line {
  local CH='─'
  local TEXT="${1//\*/${CH}}"
  local LINE_LENGTH="$(($(tput cols) - ${#TEXT} - 1))"
  echo -ne "$TEXT"
  printf "${CH}%.0s" $(seq -s ' ' 0 "$LINE_LENGTH")
  echo
}

function log {
  echo "$(echo -ne "\e[38;5;4m$(basename "$0"):\e[m") $*"
}

function logerror {
  log "$(echo -ne '\e[38;5;1merror:\e[m') $*" >&2
}

function get-build-type {
  case "${1,,}" in
  debug)
    echo "Debug"
    ;;
  release)
    echo "Release"
    ;;
  rwd | relwithdebinfo)
    echo "RelWithDebInfo"
    ;;
  msr | minsizerel)
    echo "MinSizeRel"
    ;;
  *)
    return 2
    ;;
  esac
  return 0
}

function print-help {
  echo "Usage: $(basename "$0") [options]"
  echo
  echo "Options:"
  echo "      --clean                 Cleans the build directory."
  echo "  -c, --config={debug|release|relwithdebinfo|rwd|minsizerel|msr}"
  echo "                              Specifies the build configuration. If not specified, the default value is 'debug'"
  echo "  -j, --parallel=<jobs>       Builds in parallel with <jobs> number of jobs. If not"
  echo "                               specified, the default value is nproc + 1."
  echo "      --cmake-options <options>"
  echo "                              Passes <options> to CMake before generating build files."
  echo "  -o, --output-directory <dir>"
  echo "                              Specifies the output directory for the build output."
  echo "                               If not specified, the default directory is 'bin/<config>'"
  echo "  -h, --help            Display this message and exit."
  echo
  echo "Exit Codes:"
  echo "  0 if build was successful,"
  echo "  1 if there was an error while generating build files,"
  echo "  2 if there was an error while building, or"
  echo "  3 if there was an error otherwise."
}

##################################################################################################################
# parse command-line options
declare build_output_directory
declare extra_cmake_flags
cmake_build_type=Debug
num_jobs=$(($(nproc) + 1))
target="hermes"
options=$(getopt -a -o "hc:j:o:" -l "help,clean,config:,parallel:,cmake-options,output-directory:" -- "$@")
errc=$?
[[ $errc -eq 0 ]] && eval set -- "$options" || exit 3
while [ $# -gt 0 ]; do
  case $1 in
  -h | --help)
    print-help
    exit 0
    ;;
  --clean)
    target='clean'
    ;;
  -c | --config)
    if ! cmake_build_type=$(get-build-type "$2"); then
      logerror "unknown build configuration '$2'"
    fi
    shift
    ;;
  --cmake-options)
    extra_cmake_flags="$2"
    shift
    ;;
  -j | --jobs)
    num_jobs=$2
    shift
    ;;
  -o | --output-directory)
    build_output_directory=$(realpath "$2")
    shift
    ;;
  --)
    shift
    break
    ;;
  -*)
    logerror "unrecognized option '$1'"
    exit 3
    ;;
  *)
    break
    ;;
  esac
  shift
done
# there should be no more arguments
if [ $# -ne 0 ]; then
  for arg in "$@"; do
    logerror "invalid argument '$arg'."
  done
  exit 3
fi

##################################################################################################################

# run clean if '--clean' option is specified
if [[ "$target" == clean ]]; then
  log "cleaning build directory..."
  if cmake --build build --target clean >/dev/null 2>&1; then
    log "successfully cleaned build directory."
    exit 0
  else
    logerror "cleaning build directory failed."
    exit 3
  fi
fi

# otherwise run cmake and build
log "generating build files for '${cmake_build_type,,}'..."
draw-line '*CMake'
mkdir -p build
cmake \
  -S . \
  -B build \
  -G "Unix Makefiles" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE="$cmake_build_type" \
  -DHERMES_OUTPUT_DIR="${build_output_directory:-"$PWD/bin/${cmake_build_type,,}"}" \
  ${extra_cmake_flags:+}
errc=$?
draw-line
if [[ $errc -eq 0 ]]; then
  log "successfully generated build files."
else
  logerror "generating build files failed (with cmake exit code $errc)."
  exit 1
fi

log "building target '${target}'..."
draw-line '*Build'
cmake \
  --build build \
  -t "${target}" \
  -j "$num_jobs"
errc=$?
draw-line

if [[ 0 -ne $errc ]]; then
  logerror "build failed (with cmake exit code $errc)."
  exit 2
else
  log "successfully built target '${target}'."
  exit 0
fi
##################################################
