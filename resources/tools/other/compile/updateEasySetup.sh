#!/bin/bash
source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

cd resources/tools/
./generate.sh
cd ../../

~/.platformio/penv/bin/pio run -e $pio_env

# Rust
cd components/rusty/inkrusty
cargo update
cd ../../../
