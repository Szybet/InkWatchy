#!/bin/bash

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

# https://github.com/earlephilhower/mklittlefs/blob/6e2fa1789757d52455e08f9deb33b8992ba96e07/main.cpp#L817-L818
# Block size adjusted for book
# https://electronics.stackexchange.com/questions/588293/esp32-littlefs-change-block-size
# Fuck
./in/mklittlefs --all-files -c littlefs -s $size out/fs.bin
../other/in/esptool write_flash $offset out/fs.bin