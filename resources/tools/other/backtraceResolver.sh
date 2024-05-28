#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/tasks.json)

XTENSA_ADDR2LINE="$HOME/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-addr2line"
ELF_FILE=".pio/build/$pio_env/firmware.elf"

IFS=' ' read -r -a BACKTRACES <<< "$1"

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
