#!/bin/bash

size=$(<out/size.txt tr -d '\n')
offset=$(<out/offset.txt tr -d '\n')

./mklittlefs -c out/fs_dump -s $size out/fs.bin
esptool.py write_flash $offset out/fs.bin