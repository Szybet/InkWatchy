#!/bin/bash

if [ ! -f "other/in/magick" ]; then
    echo "Downloading image magick"
    wget -q -O magick https://github.com/ImageMagick/ImageMagick/releases/download/7.1.1-34/ImageMagick-b0b7b17-gcc-x86_64.AppImage
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

if [ ! -f "fs/in/mklittlefs" ]; then
    echo "Downloading mklittlefs"
    wget -q -O mklittlefs.tar.gz https://github.com/earlephilhower/mklittlefs/releases/download/4.0.0/x86_64-linux-gnu-mklittlefs-2f7654f.tar.gz
    tar -xf mklittlefs.tar.gz
    chmod +x mklittlefs/mklittlefs
    mv mklittlefs/mklittlefs fs/in/
    rm -rf mklittlefs
    rm -rf mklittlefs.tar.gz
    
    #wget -q -O mklittlefs https://github.com/Szybet/mklittlefs/releases/download/3.2.0/mklittlefs
    #chmod +x mklittlefs
    #mv mklittlefs fs/in/
fi

if [ ! -f "other/in/esptool" ]; then
    echo "Downloading esptool"
    rm -rf esptool-download
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

if [ ! -d "/root/esp-idf/package.json" ]; then
    echo "Getting esp idf"
    current_path=$(pwd)
    cd /root/
    rm -rf esp-idf-git/
    mkdir esp-idf-git
    cd esp-idf-git/
    wget -q -O esp-idf.zip https://github.com/Szybet/esp-idf-platformio/archive/refs/heads/main.zip
    unzip esp-idf.zip
    mv esp-idf-platformio-main/* .
    rm -rf esp-idf.zip
    cd ..

    # rm -rf esp-idf-git/.git # not sure about this one, if it will make problems for gitignore and vscode git support or smth
    mv esp-idf-git/{.*,*} esp-idf/
    rm -rf esp-idf-git
    cd esp-idf/
    chmod +x install.sh
    ./install.sh
    cd $current_path
fi

if [ ! -d "components/arduino" ]; then
    echo "Getting arduino core"
    cd components/
    rm -rf arduino
    mkdir arduino
    cd arduino
    wget -q -O arduino.zip https://github.com/Szybet/arduino-esp32/archive/refs/heads/master.zip
    unzip arduino.zip
    rm -rf arduino.zip
    cd ../../
fi