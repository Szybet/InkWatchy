#!/bin/bash
cp -r resources/demo/* resources/personal/
cd resources/
export PLATFORMIO_ENV_NAME="min"
./generate.sh
