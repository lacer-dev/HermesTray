#!/usr/bin/env bash

clang-format -i $(find ./src/ -name '*.cpp' -o -name '*.h')