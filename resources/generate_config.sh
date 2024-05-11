#!/bin/bash

cd ..
git submodule update --init --recursive
if [ ! -f "src/defines/config.h" ] || [ ! -f "src/defines/confidential.h" ]; then
    echo -e ''
    echo "Creating src config"
    cd src/defines/templates/
    cp gifnoc-template.h ../config.h
    cp laitnedifnoc-template.h ../confidential.h
    cd ../../../
fi
cd resources/
if [ ! -f "fs/fsConfig.ini" ] || [ ! -f "fs/partitions.csv" ]; then
    echo -e ''
    echo "Creating fs config"
    cd fs/templates/
    cp gifnoCsf-template.ini ../fsConfig.ini
    cp snoititrap-template.csv ../partitions.csv
    cd ../../
fi