#!/bin/bash
source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

# We need it to compile the program that creates fonts
pio pkg update -e $pio_env

cd resources/tools/
./generate.sh
cd ../../

pio run -e $pio_env

# Rust
cd components/rusty/inkrusty
cargo update
cd ../../../