#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

cd .pio/build/$pio_env

#ninja -k 0 || true
#exit 0

check_and_run() {
    TARGET="$1"
    FILE="$2"

    if [ ! -f "$FILE" ]; then
        echo "Missing $FILE" >&2
        ninja "$TARGET" -k 0 || true
    fi
}

check_and_run "https_server.crt.S" "https_server.crt.S"

check_and_run "rmaker_mqtt_server.crt.S" "rmaker_mqtt_server.crt.S"

check_and_run "rmaker_claim_service_server.crt.S" "rmaker_claim_service_server.crt.S"

check_and_run "rmaker_ota_server.crt.S" "rmaker_ota_server.crt.S"

#FILE="~/.platformio/penv/.espidf-5.1.2/lib/python3.12/site-packages/pyserial-3.5.dist-info"

#if [ ! -e "$FILE" ]; then
#    echo "Pyserial doesn't exist, trying to install it..."
#    /root/.platformio/penv/.espidf-5.1.2/bin/python -m pip install pyserial
#fi

