#!/bin/bash

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

./in/mklittlefs -c out/fsDump -s $size out/fsPers.bin
esptool.py write_flash $offset out/fsPers.bin