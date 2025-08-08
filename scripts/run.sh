#!/usr/bin/env bash

set -euo pipefail

program_name=Minecraft

(
    # Set directory to root folder
    cd "$(dirname "$0")/.."

    # Case insensitive argument
    build_type=$(echo "$1" | tr '[:upper:]' '[:lower:]')

    if [[ $# -eq 1 && ( "$build_type" == "release" || "$build_type" == "debug" ) ]]; then
        ./scripts/build.sh "$build_type"
    else
        echo "Usage: ./run.sh <release|debug>"
        exit 1
    fi

    ./bin/"$build_type"/"$program_name"
)




