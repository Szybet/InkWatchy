#!/bin/bash

if [ ! -f "other/in/magick" ]; then
    echo "Downloading image magick"
    wget -q -O magick https://github.com/ImageMagick/ImageMagick/releases/download/7.1.1-34/ImageMagick-b0b7b17-gcc-x86_64.AppImage
    chmod +x magick
    # - If your resource generation doesn't work, you get errors about magick "file not found" go to `resources/tools/other/in` and execute `dd if=/dev/zero bs=1 count=3 seek=8 conv=notrunc of=magick`. Appimages are stupid.
    dd if=/dev/zero bs=1 count=3 seek=8 conv=notrunc of=magick
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

current_path=$(pwd)
# if [ ! -f "/root/esp-idf/package.json" ]; then
#     echo "Getting esp idf"
#     cd /root/
#     rm -rf esp-idf-git/
#     mkdir esp-idf-git
#     cd esp-idf-git/
#     wget -q -O esp-idf.zip https://github.com/pioarduino/esp-idf/releases/download/v5.1.4.240805/esp-idf-v5.1.4.zip
#     unzip esp-idf.zip
#     mv esp-idf-v5.1.4.240805/* .
#     rm -rf esp-idf-v5.1.4.240805
#     rm -rf esp-idf.zip
#     cd ..
#     sync

#     # rm -rf esp-idf-git/.git # not sure about this one, if it will make problems for gitignore and vscode git support or smth
#     # mv esp-idf-git/{.*,*} esp-idf/
#     mv esp-idf-git/* esp-idf/
#     rm -rf esp-idf-git
#     cd esp-idf/
#     chmod +x install.sh
#     ./install.sh
#     rm -rf .git
# fi
# cd $current_path

cd ../../
rm -rf .esp-idf/esp-idf
ln -s $PWD/.platformio/packages/framework-espidf $PWD/.esp-idf/esp-idf
cd $current_path

if [ ! -d "../../components/arduino/docs" ]; then
    echo "Getting arduino core"
    cd ../../
    mkdir -p components/arduino
    cd components
    rm -rf arduino
    mkdir arduino
    cd arduino
    #wget -q -O arduino.zip https://github.com/Szybet/arduino-esp32/archive/refs/heads/idf-release/v5.3.zip
    wget -q -O arduino.zip https://github.com/espressif/arduino-esp32/archive/refs/tags/3.1.0-RC1.zip
    unzip arduino.zip
    mv arduino-esp32-*/* .
    rm -rf arduino-esp32-*
    rm -rf arduino.zip
    cd ../../
fi
cd $current_path