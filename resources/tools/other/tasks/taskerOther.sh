#!/bin/bash

source resources/tools/globalFunctions.sh

OPTIONS=("1" "Patch wifi" 
        "2" "Create demo image"
        "3" "Launch watchy-scom"
        "4" "Compile, Upload, Scom")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=40
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="Other options"
MENU="Choose:"

CHOICE=$(dialog --clear \
                --backtitle "$TITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

clear
case $CHOICE in
    1)
        cd resources/tools/other/wifiTool/ && ./patch.sh
        ;;
    2)
        resources/tools/other/compile/demoMaker.sh
        ;;
    3)
        baudrate=$(extract_monitor_speed platformio.ini)
        device=$(extract_serial_port resources/tools/other/in/esptool)
        cd ../watchy-scom/watchy-scom
        cargo run --release -- -b $baudrate -p $device
        ;;
    4)
        baudrate=$(extract_monitor_speed platformio.ini)
        device=$(extract_serial_port resources/tools/other/in/esptool)
        pio_env=$(get_pio_env .vscode/launch.json)
        pio run -e $pio_env -t upload --upload-port $device
        cd ../watchy-scom/watchy-scom
        cargo run --release -- -b $baudrate -p $device
        ;;
esac
