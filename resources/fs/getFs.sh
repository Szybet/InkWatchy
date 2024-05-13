#!/bin/bash

size=$(<out/size.txt tr -d '\n')
offset=$(<out/offset.txt tr -d '\n')

rm -rf out/fs_dump
mkdir -p out/fs_dump
esptool.py read_flash $offset $size out/fs_dump.bin
./mklittlefs -u out/fs_dump out/fs_dump.bin