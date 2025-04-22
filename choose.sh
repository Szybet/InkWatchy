#!/bin/bash

# Define menu options
OPTIONS=(
  1 "Watchy 1"
  2 "Watchy 1.5"
  3 "Watchy 2"
  4 "Watchy 3"
  5 "Yatchy"
)

# Menu step
CHOICE=$(dialog --title "Choose your Watchy" \
    --menu "Select an item to export:" \
    15 40 5 \
    "${OPTIONS[@]}" \
    2>&1 >/dev/tty)

clear

case "$CHOICE" in
  1) export ENVNAME="Watchy_1" ;;
  2) export ENVNAME="Watchy_1_5" ;;
  3) export ENVNAME="Watchy_2" ;;
  4) export ENVNAME="Watchy_3" ;;
  5) export ENVNAME="Yatchy" ;;
  *) echo "No valid option selected."; exit 1 ;;
esac

echo "You chose $ENVNAME"



# Define menu options
OPTIONS=(
  1 "Resources (regenerate and flash images or video)"
  2 "Get and upload Calendar"
  3 "Recompile source code"
  4 "upload code to watchy"
  5 "recomplie and immidietly upload"
)

# Menu step
CHOICE=$(dialog --title "What do you want to do?" \
    --menu "Select an item:" \
    15 40 5 \
    "${OPTIONS[@]}" \
    2>&1 >/dev/tty)

clear

case "$CHOICE" in
  1) PLATFORMIO_ENV_NAME=$ENVNAME ./resources/tools/other/tasks/taskerResources.sh ;;
  2) PLATFORMIO_ENV_NAME=$ENVNAME ./resources/tools/other/tasks/taskerOther.sh ;;
  3) pio run -e $ENVNAME ;;
  4) pio run -t upload -e $ENVNAME ;;
  5) pio run -e $ENVNAME && pio run -t upload -e $ENVNAME ;;
  *) echo "No valid option selected."; exit 1 ;;
esac

echo "You chose $ENVNAME"

exec bash
