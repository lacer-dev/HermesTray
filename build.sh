#!/usr/bin/env bash

pushd build 1>/dev/null
ninja
NINJA_EXIT_CODE=$?
popd 1>/dev/null

if [[ ! $NINJA_EXIT_CODE ]]; then
    echo "build.sh: error: ninja failed with exit code $NINJA_EXIT_CODE"
    exit $NINJA_EXIT_CODE
fi

exit 0