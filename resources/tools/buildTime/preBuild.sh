#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

cd .pio/build/$pio_env
ninja || true
ninja || true
ninja || true