#!/bin/bash -x

OPTIONS=("1" "Generate resources"
         #"2" "Generate partition table"
         "3" "Flash filesystem"
         "4" "Get filesystem from device"
         "5" "Flash filesystem you got from the device"
         "6" "(COULD FAIL) Fast resource generation + flash")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=60
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="Resources options"
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
        cd resources/tools/ && ./generate.sh
        ;;
    2)
        cd resources/tools/fs/ && python3 generatePartTable.py
        ;;
    3)
        cd resources/tools/fs/ && ./flashFs.sh
        ;;
    4)
        cd resources/tools/fs/ && ./getFs.sh
        ;;
    5)
        cd resources/tools/fs/ && ./flashPersonalFs.sh
        ;;
    6)
        cd resources/tools/ && ./generate_fast.sh
        cd fs/ && ./flashFs.sh
        ;;
esac
