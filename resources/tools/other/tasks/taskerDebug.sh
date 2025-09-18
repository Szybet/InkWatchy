#!/bin/bash

source resources/tools/globalFunctions.sh

OPTIONS=("1" "Reset ESP"
         "2" "Reset and monitor"
         "3" "Backtrace resolver"
         "4" "Get coredump"
         "5" "Erase ESP flash"
         "6" "Put device into bootloader mode")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=60
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="Debug options"
MENU="Choose:"

CHOICE=$(dialog --clear \
                --backtitle "$TITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

clear

esptool="resources/tools/other/in/esptool"

case $CHOICE in
    1)
        serial_port=$(extract_serial_port "$esptool")
        if [[ $? -ne 0 || -z "$serial_port" ]]; then
            echo "Failed to detect a valid serial port" >&2
            exit 1
        fi

        $esptool --port $serial_port --before default_reset chip_id
        ;;
    2)
        cd resources/tools/other/debug/
        ./resetAndMonitor.sh
        ;;
    3)  
        args=$(dialog --inputbox "Enter the backtrace" 8 150 3>&1 1>&2 2>&3)
        clear
        ./resources/tools/other/debug/backtraceResolver.sh $args
        ;;
    4)
        cd resources/tools/other/debug/
        ./getCoreDump.sh
        ;;
    5)
        serial_port=$(extract_serial_port "$esptool")
        if [[ $? -ne 0 || -z "$serial_port" ]]; then
            echo "Failed to detect a valid serial port" >&2
            exit 1
        fi
        $esptool --port $serial_port erase_flash
        ;;
    6)
        serial_port=$(extract_serial_port "$esptool")
        if [[ $? -ne 0 || -z "$serial_port" ]]; then
            echo "Failed to detect a valid serial port" >&2
            exit 1
        fi
        $esptool --port $serial_port --after no_reset chip_id
        ;;
esac
