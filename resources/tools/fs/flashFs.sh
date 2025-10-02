#!/bin/bash -x

size=$(<in/size.txt tr -d '\n')
offset=$(<in/offset.txt tr -d '\n')

# https://github.com/earlephilhower/mklittlefs/blob/6e2fa1789757d52455e08f9deb33b8992ba96e07/main.cpp#L817-L818
# Block size adjusted for book
# https://electronics.stackexchange.com/questions/588293/esp32-littlefs-change-block-size
# Fuck

source ../globalFunctions.sh
serial_port=$(extract_serial_port "../other/in/esptool")
if [[ $? -ne 0 || -z "$serial_port" ]]; then
    echo "Failed to detect a valid serial port" >&2
    exit 1
fi

mv ./littlefs/other/yatchy-lp-program*.bin ./littlefs/other/yatchy-lp-program.bin 2>/dev/null || true
rm -rf /tmp/littlefs
cp -r littlefs /tmp/

find /tmp/littlefs -type f ! -path "*/book/*" ! -name ".keep" -exec sh -c 'echo "Compressing: $1"; tamp compress "$1" > "$1.tmp" && mv "$1.tmp" "$1"' _ {} \;
rm -rf out/fs.bin
./in/mklittlefs --all-files -c /tmp/littlefs -s $size out/fs.bin
rm -rf /tmp/littlefs

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

../other/in/esptool --port $serial_port write_flash $offset out/fs.bin
