#!/bin/bash
source resources/tools/globalFunctions.sh

# We need it to compile the program that creates fonts
pio pkg install -e min

cd resources/tools/
export PLATFORMIO_ENV_NAME="min"
./generate.sh
cd ../../

pio run -e min

cd resources/tools/fs/
PLATFORMIO_ENV_NAME="min" python3 generatePartTable.py
cd ../../../

pio run -e min