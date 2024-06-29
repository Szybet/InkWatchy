#!/bin/bash
source resources/tools/globalFunctions.sh

pio pkg install
rm -rf resources/personal/books/* resources/personal/vault/*
cp -r resources/demo/* resources/personal/

cd resources/tools/
./generate.sh
cd ../../

serial_port=$(extract_serial_port resources/tools/other/in/esptool)
echo "Serial port detected: $serial_port"

old_string="#define VAULT_PASSWORD \"\""
new_string="#define VAULT_PASSWORD \"1\""
filename="src/defines/confidential.h"

sed -i "s/$old_string/$new_string/g" "$filename"

old_string="#define VAULT 0"
new_string="#define VAULT 1"
filename="src/defines/config.h"

sed -i "s/$old_string/$new_string/g" "$filename"

old_string="#define BOOK 0"
new_string="#define BOOK 1"
filename="src/defines/config.h"

sed -i "s/$old_string/$new_string/g" "$filename"

old_string="#define WIFI_SSID1 \"\""
new_string="#define WIFI_SSID1 \"hotspot\""
filename="src/defines/confidential.h"

sed -i "s/$old_string/$new_string/g" "$filename"

old_string="#define WIFI_PASS1 \"\""
new_string="#define WIFI_PASS1 \"12345678\""
filename="src/defines/confidential.h"

sed -i "s/$old_string/$new_string/g" "$filename"

# To apply those things
cd resources/tools/
./generate.sh
cd ../../

pio run

#pio run
#cd resources/tools/fs/
#python3 generatePartTable.py
#cd ../../../

# This will succeed
pio run
resources/tools/other/in/esptool -p $serial_port erase_flash
pio run -t upload --upload-port $serial_port

sleep 10

cd resources/tools/fs/
./flashFs.sh
cd ../../../

sleep 10

cd resources/tools/other/
in/esptool -p $serial_port read_flash 0x00000 0x400000 out/demo-program.bin
#python ../fs/cleanPartition.py ../fs/in/partitions.csv out/demo-program.bin nvs
#python ../fs/cleanPartition.py ../fs/in/partitions.csv out/demo-program.bin coredump
sleep 5
in/esptool -p $serial_port erase_flash
sleep 5
in/esptool -p $serial_port write_flash 0x00000 out/demo-program.bin
cd ../../../
zip -r -9 resources/tools/other/out/demo-source.zip . 1>/dev/null 2>/dev/null