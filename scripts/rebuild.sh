#!/usr/bin/env bash

set -euo pipefail

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

    rm -rf build
    mkdir -p build
    cmake -G "$generator" -B build
)
