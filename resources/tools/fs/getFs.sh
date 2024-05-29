#!/bin/bash

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

rm -rf out/fsDump fsDump.bin
mkdir -p out/fsDump
../other/in/esptool read_flash $offset $size out/fsDump.bin
./in/mklittlefs -u out/fsDump out/fsDump.bin