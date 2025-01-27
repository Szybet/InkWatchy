#!/bin/bash -x

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

# https://github.com/earlephilhower/mklittlefs/blob/6e2fa1789757d52455e08f9deb33b8992ba96e07/main.cpp#L817-L818
# Block size adjusted for book
# https://electronics.stackexchange.com/questions/588293/esp32-littlefs-change-block-size
# Fuck

rm -rf out/fs.bin
./in/mklittlefs --all-files -c littlefs -s $size out/fs.bin

if [ $? -ne 0 ]; then
    echo "Command failed. Exiting the script."
    exit 1
fi

# rm -rf out/fs.bin
# dd if=/dev/zero of=out/fs.bin bs=$size count=1
# sudo losetup /dev/loop84 out/fs.bin
# sudo lfs --block_size=4096 --format /dev/loop84
# rm -rf in/fsMount
# mkdir in/fsMount
# sudo lfs --block_size=4096 /dev/loop84 in/fsMount
# sudo cp -r littlefs/* in/fsMount/
# sync
# sudo umount in/fsMount/
# sync
# sudo losetup -d /dev/loop84

../other/in/esptool write_flash $offset out/fs.bin