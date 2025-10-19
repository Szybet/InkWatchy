#!/bin/bash

source ../../globalFunctions.sh

serial_port=$(extract_serial_port "../in/esptool")
if [[ $? -ne 0 || -z "$serial_port" ]]; then
    echo "Failed to detect a valid serial port" >&2
    exit 1
fi
echo "Selected serial port: $serial_port"
#../in/esptool --before default_reset chip_id # Not needed as extracting serial port resets
~/.platformio/penv/bin/pio device monitor -p $serial_port -b 115200
