#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

toolchain_dir=""
addr_file=""

if [[ "$pio_env" =~ 3 ]]; then
    toolchain_dir="xtensa-esp-elf"
    addr_file="xtensa-esp32s3-elf-addr2line"
elif [[ "$pio_env" =~ 2 ]]; then
    toolchain_dir="xtensa-esp-elf"
    addr_file="xtensa-esp32-elf-addr2line"
fi

XTENSA_ADDR2LINE="$HOME/.platformio/packages/$toolchain_dir/bin/$addr_file"
ELF_FILE=".pio/build/$pio_env/firmware.elf"

IFS=' ' read -r -a BACKTRACES <<< "$@" # <<< "$1" 

for BACKTRACE in "${BACKTRACES[@]}"; do
    echo "Processing backtrace: $BACKTRACE"

    while IFS= read -r LINE; do
        ADDR="$(echo "$LINE" | awk '{print $1}')"
        OFFSET="$(echo "$LINE" | awk '{print $2}')"
        if [ "$OFFSET" = "0x0" ]; then
            continue
        fi
        echo -n "$ADDR: "
        "$XTENSA_ADDR2LINE" -e "$ELF_FILE" "$ADDR"
    done < <(echo "$BACKTRACE")
    echo
done
