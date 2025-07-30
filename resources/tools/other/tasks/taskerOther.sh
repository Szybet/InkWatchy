#!/bin/bash

source resources/tools/globalFunctions.sh

OPTIONS=("1" "Patch wifi" 
         "2" "Create demo image"
         "3" "Launch watchy-scom"
         "4" "Compile, Upload, Scom"
         "5" "Get calendar events and upload them")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))
HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=60
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
        dialog --yesno "Do you want to clean and regenerate config?" 7 50
        cleanConfig=$?

        envList=("Watchy_1" "Watchy_1_5" "Watchy_2" "Watchy_3" "Yatchy" "All")
        envChoice=$(dialog --clear \
                    --title "Select environment" \
                    --menu "Environment:" 15 50 6 \
                    $(for i in "${!envList[@]}"; do echo "$i ${envList[$i]}"; done) \
                    2>&1 >/dev/tty)
        clear

        selectedEnv="${envList[$envChoice]}"

        if [[ "$selectedEnv" == "All" ]]; then
            if [[ "$cleanConfig" == 1 ]]; then
                echo "you have to clean config if you want all"
                exit
            fi
            resources/tools/other/compile/demoMaker.sh "" "$cleanConfig"
        else
            if [[ "$cleanConfig" == 0 ]]; then
                if [[ "$selectedEnv" == "Yatchy" ]]; then
                    accChoice=$(dialog --clear \
                                --title "Accelerometer" \
                                --menu "Select accelerometer option:" 15 50 4 \
                                1 "Use BMA456" \
                                2 "Use BMA530" \
                                3 "Disable accelerometer" \
                                2>&1 >/dev/tty)
                    clear
                else
                    dialog --yesno "Do you want to disable the accelerometer?" 7 50
                    if [[ $? == 0 ]]; then
                        accChoice=3
                    else
                        accChoice=0
                    fi
                fi
            else
                accChoice=""
            fi
            resources/tools/other/compile/demoMaker.sh "$selectedEnv" "$cleanConfig" "$accChoice"
        fi
        ;;
    3)
        baudrate=$(extract_monitor_speed platformio.ini)
        device=$(extract_serial_port resources/tools/other/in/esptool)
        cd src/other/scomTask/watchy-scom/
        ./generate_symlinks.sh
        cd watchy-scom/
        cargo run --release -- -b $baudrate -p $device
        ;;
    4)
        baudrate=$(extract_monitor_speed platformio.ini)
        device=$(extract_serial_port resources/tools/other/in/esptool)
        pio_env=$(get_pio_env .vscode/launch.json)
        pio run -e $pio_env -t upload --upload-port $device
        cd src/other/scomTask/watchy-scom/
        ./generate_symlinks.sh
        cd watchy-scom/
        cargo run --release -- -b $baudrate -p $device
        ;;
    5)
        resources/tools/other/calendar/getEvents.sh
        ;;
esac
