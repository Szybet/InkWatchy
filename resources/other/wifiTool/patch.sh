#!/bin/sh
if [ $# -lt 1 ]; then
    file=../../../.pio/build/ink_watchy_wifi_tool/firmware.elf
    if [ ! -e "$file" ]; then
        echo "File doesn't exist, using max path"
        file=../../../.pio/build/max/firmware.elf
    fi
else
    file=$1
fi

if [ ! -r $file ]; then
    echo "ELF file $file doesn't exist or isn't readable"
    exit 1
fi

if [ $# -lt 2 ]; then
    patch=ret0.bin
else
    patch=$2
fi

if [ ! -r $patch ]; then
    echo "Patch file $patch doesn't exist or isn't readable"
    exit 1
fi

sanity_check_virt=$(readelf -s $file | grep ieee80211_raw_fr | awk '{ print "0x" $2 }')
sanity_check_base=$(readelf -S $file | grep flash.text | awk '{ print "0x"$4 " + 0x" $5 }')
offset=$(python -c "print($sanity_check_virt - $sanity_check_base)")
size=$(stat --printf="%s" $patch)

dd conv=notrunc if=$patch of=$file bs=1 count=$size seek=$offset
echo "Now recompile the binary without any changes"
