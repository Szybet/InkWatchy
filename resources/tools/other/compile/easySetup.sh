#!/bin/bash
source resources/tools/globalFunctions.sh

# We need it to compile the program that creates fonts
pio pkg install

cd resources/tools/
./generate.sh
cd ../../

pio run
pio run