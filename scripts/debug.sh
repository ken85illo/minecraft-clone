#!/usr/bin/env bash

set -euo pipefail

program_name=Minecraft

(
    # Set directory to root folder
    cd "$(dirname "$0")/.."
    ./scripts/build.sh "debug"
    gdb ./bin/debug/"$program_name"
)
