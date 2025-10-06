#!/bin/bash

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

if ! check_define FILESYSTEM_COMPRESSION ../../../src/defines/config.h; then
    echo "FILESYSTEM_COMPRESSION is enabled. Applying compression..."
    # Compile the C compressor
    echo "Compiling compressFile.c..."
    gcc -o in/compressFile -I../../../components/tamp/ ../../../components/tamp/tamp/common.c ../../../components/tamp/tamp/compressor.c compressFile.c
    if [ $? -ne 0 ]; then
        echo "Error compiling compressFile.c. Exiting."
        exit 1
    fi

    find /tmp/littlefs -type f ! -path "*/book/*" ! -name ".keep" -exec bash -c './in/compressFile "$0" "$0.compressed" && mv "$0.compressed" "$0"' {} \;
else
    echo "FILESYSTEM_COMPRESSION is not enabled. Skipping compression."
fi

rm -rf out/fs.bin
mklittlefs_output=$(./in/mklittlefs --all-files -c /tmp/littlefs -s $size out/fs.bin 2>&1)
rm -rf /tmp/littlefs

if echo "$mklittlefs_output" | grep -q "No more free space"; then
    echo "Error: Filesystem is too big and won't fit" >&2
    exit 1
fi

if [ ! -f out/fs.bin ]; then
    echo "'out/fs.bin' was not created." >&2
    exit 1
fi

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
