#!/bin/bash

OPTIONS=("1" "Run Command 1"
         "2" "Run Command 2"
         "3" "Run Command 3"
         "4" "Run Command 4"
         "5" "Run Command 5")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=40
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="Options"
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

        ;;
    2)

        ;;
    3)

        ;;
    4)

        ;;
    5)

        ;;
esac
