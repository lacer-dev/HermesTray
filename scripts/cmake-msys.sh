#!/bin/bash

pushd ../build
cmake -B . -S .. -G "Ninja"
popd