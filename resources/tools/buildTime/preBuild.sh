#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

cd .pio/build/$pio_env
ninja -k 0 || true
#ninja || true
#ninja || true


FILE="~/.platformio/penv/.espidf-5.1.2/lib/python3.12/site-packages/pyserial-3.5.dist-info"

if [ ! -e "$FILE" ]; then
    echo "Pyserial doesn't exist, trying to install it..."
    /root/.platformio/penv/.espidf-5.1.2/bin/python -m pip install pyserial
fi
