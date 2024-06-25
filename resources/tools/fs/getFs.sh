#!/bin/bash

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

rm -rf fsDump.bin
../other/in/esptool read_flash $offset $size out/fsDump.bin

./in/mklittlefs -u out/fsDump out/fsDump.bin

# sudo losetup /dev/loop86 out/fsDump.bin
# rm -rf out/fsMountPersonalTmp
# rm -rf out/fsMountPersonal
# mkdir -p out/fsMountPersonalTmp
# sudo lfs --block_size=4096 /dev/loop86 out/fsMountPersonalTmp
# sudo cp -r out/fsMountPersonalTmp/* out/fsMountPersonal/
# sudo umount out/fsMountPersonalTmp/
# sync
# sudo losetup -d /dev/loop86