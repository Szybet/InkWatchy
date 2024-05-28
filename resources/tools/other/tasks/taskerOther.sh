#!/bin/bash

OPTIONS=("1" "Patch wifi" "2" "Create demo image")

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
        resources/tools/other/demoMaker.sh
        ;;
esac
