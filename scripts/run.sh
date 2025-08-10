#!/usr/bin/env bash

set -euo pipefail

program_name=Minecraft

if [[ $# -ne 1 ]]; then
    echo "Usage: ./run.sh <release|debug>"
    exit 1
fi

# Case insensitive argument
build_type=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [[ "$build_type" != "release" && "$build_type" != "debug" ]]; then
    echo "Usage: ./run.sh <release|debug>"
    exit 1
fi


(
    # Set directory to root folder
    cd "$(dirname "$0")/.."
    ./scripts/build.sh "$build_type"
    ./bin/"$build_type"/"$program_name"
)




