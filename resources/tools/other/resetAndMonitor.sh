#!/bin/bash

source ../globalFunctions.sh

serial_port=$(extract_serial_port "in/esptool")
echo "Selected serial port: $serial_port"
in/esptool --before default_reset chip_id
pio device monitor -p $serial_port -b 115200