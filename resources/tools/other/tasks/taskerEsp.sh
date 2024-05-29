#!/bin/bash

OPTIONS=("1" "Reset ESP"
         "2" "Reset and monitor"
         "3" "Backtrace resolver"
         "4" "Get coredump")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=40
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="ESP options"
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
        resources/tools/other/in/esptool --before default_reset chip_id
        ;;
    2)
        cd resources/tools/other/
        ./resetAndMonitor.sh
        ;;
    3)  
        args=$(dialog --inputbox "Enter the backtrace" 8 150 3>&1 1>&2 2>&3)
        clear
        ./resources/tools/other/backtraceResolver.sh $args
        ;;
    4)
        cd resources/tools/other
        ./getCoreDump.sh
        ;;
esac
