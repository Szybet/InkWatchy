#!/bin/bash

OPTIONS=("1" "Clean everything"
         "2" "Setup build")

NUM_OPTIONS=$((${#OPTIONS[@]} / 2))

HEIGHT=$((NUM_OPTIONS + 7))
WIDTH=40
CHOICE_HEIGHT=$((NUM_OPTIONS + 1))

TITLE="Compile options"
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
        resources/tools/other/compile/clean.sh
        ;;
    2)
        resources/tools/other/compile/clean.sh
        resources/tools/other/compile/easySetup.sh
        ;;
esac
