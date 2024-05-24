#!/bin/bash

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

rm -rf out/fsDump
mkdir -p out/fsDump
esptool.py read_flash $offset $size out/fsDump.bin
./in/mklittlefs -u out/fsDump out/fsDump.bin