#!/bin/bash

# Define menu options
OPTIONS=(
  1 "Watchy 1"
  2 "Watchy 1.5"
  3 "Watchy 2"
  4 "Watchy 3"
  5 "Yatchy"
)

echo "Downloading and installing PlatformIO..."
curl -fsSL -o /tmp/ipio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py && python3 /tmp/ipio.py
echo "Done."

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
sleep 1

# First-time question
dialog --title "First-Time Setup" --yesno "Is this your first time running the setup?" 7 50

if [ $? -eq 0 ]; then
    # User said YES
    dialog --msgbox "Please select First Build in the next step." 7 50
    PLATFORMIO_ENV_NAME=$ENVNAME ./resources/tools/other/tasks/taskerCompile.sh
else
    echo "Skipping first-time setup."
fi

dialog --title "Wanna Upload" --yesno "Do you wanna upload this new fresh build?" 7 50

if [ $? -eq 0 ]; then
    # User said YES
    pio run -t upload -e $ENVNAME
else
    echo "Skipping firmware upload"
fi


