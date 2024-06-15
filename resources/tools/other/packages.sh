#!/bin/bash

function imagemagick_from_source() {
    if [ ! -f "other/in/magick" ]; then
        echo "Downloading image magick"
        wget https://imagemagick.org/archive/binaries/magick
        chmod +x magick
        mv magick other/in/magick
        rm magick* 1>/dev/null 2>/dev/null # Not sure
    fi

    # That's for libraries, fuck
    #git clone https://github.com/ImageMagick/ImageMagick.git ImageMagick
    #cd ImageMagick
    #./configure --with-modules
    #make -j$(nproc)
    #sudo make install
    #sudo ldconfig /usr/local/lib
    #cd ..
}

imagemagick_from_source

if command -v dpkg &> /dev/null; then
    echo "Detected a debian based system"
    debian_packages=("mupdf-tools" "xxd" "jq" "python3-unidecode" "libfreetype-dev" "libfreetype6" "libfreetype6-dev" "python3-venv" "imagemagick" "libltdl-dev" "make" "build-essential" "automake")
    missing_packages=()

    for pkg in "${debian_packages[@]}"; do
        if ! dpkg -s "$pkg" &> /dev/null; then
            missing_packages+=("$pkg")
        fi
    done

    if [ ${#missing_packages[@]} -gt 0 ]; then
        echo "Installing missing packages: ${missing_packages[*]}"
        timeout 1m sudo apt-get install -y "${missing_packages[@]}"
        if [ $? -ne 0 ]; then
            echo "Failed to install some packages. It will fail."
        fi
    else
        echo "All required packages are already installed."
    fi
else
    echo "I only managed to support debian based system, feel free to add other distros :)"
    echo "Oh yea good luck :D"
fi

if [ ! -f "fs/in/mklittlefs" ]; then
    echo "Downloading mklittlefs"
    #wget -q -O mklittlefs.tar.gz https://github.com/earlephilhower/mklittlefs/releases/download/3.2.0/x86_64-linux-gnu-mklittlefs-975bd0f.tar.gz
    #tar -xf mklittlefs.tar.gz
    #chmod +x mklittlefs/mklittlefs
    #mv mklittlefs/mklittlefs fs/in/
    #rm -rf mklittlefs
    #rm -rf mklittlefs.tar.gz
    wget -q -O mklittlefs https://github.com/Szybet/mklittlefs/releases/download/3.2.0/mklittlefs
    chmod +x mklittlefs
    mv mklittlefs fs/in/
fi

if [ ! -f "other/in/esptool" ]; then
    echo "Downloading esptool"
    mkdir esptool-download
    cd esptool-download
    wget -q -O esptool.zip https://github.com/espressif/esptool/releases/download/v4.7.0/esptool-v4.7.0-linux-amd64.zip
    unzip esptool.zip
    chmod +x esptool-linux-amd64/esptool
    cp esptool-linux-amd64/esptool ../other/in/
    cd ..
    rm -rf esptool-download
fi

if ! command -v pio &> /dev/null; then
    echo "install platformio (pio command). The rest will fail."
fi