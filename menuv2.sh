#!/bin/bash

# ------------- [ Check for `dialog` and offer to install ] -------------
if ! command -v dialog &>/dev/null; then
    echo "The 'dialog' utility is not installed."

    read -p "Would you like to try to install it now? [y/N]: " REPLY
    if [[ "$REPLY" =~ ^[Yy]$ ]]; then
        INSTALL_CMD=""

        if command -v apt-get &>/dev/null; then
            INSTALL_CMD="sudo apt-get update && sudo apt-get install dialog"
        elif command -v dnf &>/dev/null; then
            INSTALL_CMD="sudo dnf install dialog"
        elif command -v pacman &>/dev/null; then
            INSTALL_CMD="sudo pacman -Sy dialog"
        elif command -v zypper &>/dev/null; then
            INSTALL_CMD="sudo zypper install dialog"
        elif command -v yum &>/dev/null; then
            INSTALL_CMD="sudo yum install dialog"
        elif command -v emerge &>/dev/null; then
            INSTALL_CMD="sudo emerge -1 dialog"
        else
            echo "Unsupported package manager. Please install 'dialog' manually."
            exit 1
        fi

        echo "Running: $INSTALL_CMD"
        eval "$INSTALL_CMD"

        if ! command -v dialog &>/dev/null; then
            echo "Installation failed or 'dialog' still not found. Exiting."
            exit 1
        fi
    else
        echo "Cannot proceed without 'dialog'. Exiting."
        exit 1
    fi
fi

# ------------- [ Begin Dialog Wizard Flow ] -------------
# Step 1: Docker check
if ! command -v docker &>/dev/null; then
    dialog --backtitle "Wizard Setup" --title "Docker Check" \
        --msgbox "Docker is NOT installed on this system.\n\nPlease install Docker before proceeding." 10 50
    clear
    echo "Docker not installed. Exiting."
    exit 1
fi

DOCKER_STATUS=$(systemctl is-active docker)
if [[ "$DOCKER_STATUS" != "active" ]]; then
    dialog --backtitle "Wizard Setup" --title "Docker Check" \
        --msgbox "Docker is installed but NOT running.\n\nPlease start Docker before proceeding." 10 50
    clear
    echo "Docker not running. Exiting."
    exit 1
fi

# Step 2: Info
dialog --title "Docker image Build" \
    --msgbox "This step will build a docker image\n\nPress OK to proceed." 10 50
clear
docker build resources/tools/other/compile/ --tag InkWatchy/main
sleep 1


dialog --title "Wanna run first time setup or Choose" --yesno "Run first time setup?" 7 50

if [ $? -eq 0 ]; then
    # User said YES
    docker run --privileged --mount type=bind,source=/dev/,target=/dev/,consistency=consistent --mount type=bind,source=$(pwd)/.platformio/,target=/root/.platformio/,consistency=consistent --mount type=bind,source=$(pwd)/.esp-idf/espressif/,target=/root/.espressif/,consistency=consistent --mount type=bind,source=$(pwd)/,target=/workspaces/InkWatchy/,consistency=consistent --workdir /workspaces/InkWatchy/ -it InkWatchy/main bash -c "./first.sh"
else
    docker run --privileged --mount type=bind,source=/dev/,target=/dev/,consistency=consistent --mount type=bind,source=$(pwd)/.platformio/,target=/root/.platformio/,consistency=consistent --mount type=bind,source=$(pwd)/.esp-idf/espressif/,target=/root/.espressif/,consistency=consistent --mount type=bind,source=$(pwd)/,target=/workspaces/InkWatchy/,consistency=consistent --workdir /workspaces/InkWatchy/ -it InkWatchy/main bash -c "./choose.sh"
fi

echo "seems like you are done. Exiting."

