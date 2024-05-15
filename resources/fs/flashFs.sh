#!/bin/bash

size=$(<out/size.txt tr -d '\n')
offset=$(<out/offset.txt tr -d '\n')

# https://github.com/earlephilhower/mklittlefs/blob/6e2fa1789757d52455e08f9deb33b8992ba96e07/main.cpp#L817-L818
# Block size adjusted for book
# https://electronics.stackexchange.com/questions/588293/esp32-littlefs-change-block-size
# Fuck
./mklittlefs -c littlefs -s $size out/fs.bin
esptool.py write_flash $offset out/fs.bin