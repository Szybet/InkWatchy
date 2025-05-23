#!/bin/bash

cd ../../
if [ ! -f "src/defines/config.h" ] || [ ! -f "src/defines/confidential.h" ]; then
    echo -e ''
    echo "Creating src config"
    cd src/defines/templates/
    cp gifnoc-template.h ../config.h
    cp laitnedifnoc-template.h ../confidential.h
    cd ../../../
fi
cd resources/tools/

mkdir -p fs/in/
mkdir -p fs/out/
mkdir -p fs/littlefs
mkdir -p fs/littlefs/conf/
touch fs/littlefs/conf/.keep
# TODO: Only on Yatchy selector or something
mkdir -p fs/littlefs/other/
touch fs/littlefs/other/.keep
mkdir -p fs/littlefs/calendar/

mkdir -p other/out/
mkdir -p other/in/

echo "2019328" > fs/in/offset.txt
echo "2097152" > fs/in/size.txt

if [ ! -f "fs/in/fsConfig.ini" ] || [ ! -f "fs/in/partitions.csv" ]; then
    echo -e ''
    echo "Creating fs config"
    cd fs/templates/
    cp gifnoCsf-template.ini ../in/fsConfig.ini
    cp snoititrap-template.csv ../in/partitions.csv
    cd ../../
fi