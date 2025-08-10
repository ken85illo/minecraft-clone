#!/usr/bin/env bash

set -euo pipefail


if [[ $# -ne 1 ]]; then
    echo "Usage: ./build.sh <release|debug>"
    exit 1
fi

# Case insensitive argument
build_type=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [[ "$build_type" == "release" ]]; then
    build_type="Release"
elif [[ "$build_type" == "debug" ]]; then
    build_type="Debug"
else
    echo "Usage: ./build.sh <release|debug>"
    exit 1
fi


(
    # Set directory to root folder
    cd "$(dirname "$0")/.."

    # Prioritize Ninja over Makefile
    if command -v ninja >/dev/null 2>&1; then
        generator="Ninja"
    elif command -v make >/dev/null 2>&1; then
        generator='"Unix Makefiles"'
    else
        echo "No 'ninja' or 'make' defined in PATH"
        exit 1
    fi

    if [[ ! -d "./build" ]]; then
        mkdir -p build
        cmake -G "$generator" -B build -DCMAKE_BUILD_TYPE="$build_type"
    fi

    cmake --build build
    mkdir -p bin/"$build_type"
    cp -rf build/bin/* bin/"$build_type"
)
