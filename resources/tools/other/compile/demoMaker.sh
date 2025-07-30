#!/bin/bash
source resources/tools/globalFunctions.sh

envList=("Watchy_1" "Watchy_1_5" "Watchy_2" "Watchy_3" "Yatchy")

if [[ -n "$1" ]]; then
    envList=("$1")
fi

checkForAll="$1"

cleanFlag="$2"

accChoice="$3"

function generalThings {
    cleanFlag="$1"
    accChoice="$2"
    checkForAll="$3"
    echo "clean flag" $cleanFlag
    if [[ $cleanFlag == 0 ]]; then
        mkdir trash/
        mv resources/personal/books trash/
        cp -r resources/personal/moduleImages trash/
        cp -r resources/demo/* resources/personal/
        
        echo "cleaning stuff"
        rm -rf src/defines/confidential.h
        rm -rf src/defines/config.h
        cd resources/tools/
        ./generate.sh
        cd ../../
        
        old_string="#define VAULT_PASSWORD \"\""
        new_string="#define VAULT_PASSWORD \"5\""
        filename="src/defines/confidential.h"
        
        sed -i "s/$old_string/$new_string/g" "$filename"
        
        old_string="#define VAULT 0"
        new_string="#define VAULT 1"
        filename="src/defines/config.h"
        
        sed -i "s/$old_string/$new_string/g" "$filename"
        
        old_string="#define BOOK 0"
        new_string="#define BOOK 1"
        filename="src/defines/config.h"
        
        \sed -i "s/$old_string/$new_string/g" "$filename"
        
        # old_string="#define WIFI_SSID1 \"\""
        # new_string="#define WIFI_SSID1 \"hotspot\""
        # filename="src/defines/confidential.h"
        
        # sed -i "s/$old_string/$new_string/g" "$filename"
        
        # old_string="#define WIFI_PASS1 \"\""
        # new_string="#define WIFI_PASS1 \"12345678\""
        # filename="src/defines/confidential.h"
        
        # sed -i "s/$old_string/$new_string/g" "$filename"
        
        old_string="#define WEATHER_LATIT \"\""
        new_string="#define WEATHER_LATIT \"53.55073\""
        filename="src/defines/confidential.h"
        
        sed -i "s/$old_string/$new_string/g" "$filename"
        
        old_string="#define WEATHER_LONGTIT \"\""
        new_string="#define WEATHER_LONGTIT \"9.99302\""
        filename="src/defines/confidential.h"
        
        sed -i "s/$old_string/$new_string/g" "$filename"
        
        echo "[DEBUG] Setting accelerometer config"
        if [[ "$accChoice" == "0" ]]; then
            filename="src/defines/config.h"
            old_val="1"
            new_val="0"
            
            sed -i -E "s|^[[:space:]]*#define[[:space:]]+FORCE_DISABLE_ACC[[:space:]]+${old_val}|#define FORCE_DISABLE_ACC ${new_val}|" "$filename"
            elif [[ "$accChoice" == "1" ]]; then
            bmaFile="src/defines/condition.h"
            sed -i -E 's|^[[:space:]]*//[[:space:]]*#define[[:space:]]+BMA_VERSION[[:space:]]+456|#define BMA_VERSION 456|' "$bmaFile"
            sed -i -E 's|^[[:space:]]*#define[[:space:]]+BMA_VERSION[[:space:]]+530|// #define BMA_VERSION 530|' "$bmaFile"
            filename="src/defines/config.h"
            old_val="1"
            new_val="0"
            
            sed -i -E "s|^[[:space:]]*#define[[:space:]]+FORCE_DISABLE_ACC[[:space:]]+${old_val}|#define FORCE_DISABLE_ACC ${new_val}|" "$filename"
            elif [[ "$accChoice" == "2" ]]; then
            bmaFile="src/defines/condition.h"
            sed -i -E 's|^[[:space:]]*#define[[:space:]]+BMA_VERSION[[:space:]]+456|// #define BMA_VERSION 456|' "$bmaFile"
            sed -i -E 's|^[[:space:]]*//[[:space:]]*#define[[:space:]]+BMA_VERSION[[:space:]]+530|#define BMA_VERSION 530|' "$bmaFile"
            filename="src/defines/config.h"
            old_val="1"
            new_val="0"
            
            sed -i -E "s|^[[:space:]]*#define[[:space:]]+FORCE_DISABLE_ACC[[:space:]]+${old_val}|#define FORCE_DISABLE_ACC ${new_val}|" "$filename"
            elif [[ "$accChoice" == "3" ]]; then
            filename="src/defines/config.h"
            old_val="0"
            new_val="1"
            
            sed -i -E "s|^[[:space:]]*#define[[:space:]]+FORCE_DISABLE_ACC[[:space:]]+${old_val}|#define FORCE_DISABLE_ACC ${new_val}|" "$filename"
        fi
        
        if [[ -z "$checkForAll" ]]; then
            old_string="#define LP_CORE 1"
            new_string="#define LP_CORE 0"
            filename="src/defines/condition.h"
            
            sed -i "s/$old_string/$new_string/g" "$filename"
        fi
    fi
    
    # To regenerate vault
    cd resources/tools/
    ./generate.sh
    cd ../../
    
    cd resources/tools/fs
    ./createFs.sh
    cd ../../../
}

generalThings "$cleanFlag" "$accChoice" "$checkForAll"

function compileEnv {
    pio run -e $1
}

for env in "${envList[@]}"; do
    compileEnv "$env"
done

function createEmptyBinary {
    dd if=/dev/zero of=demo.bin bs=1M count=$1
}

function hexToDec {
    printf "%d\n" "$1"
}

function writeAtOffset {
    # Input into output
    local outputFile=$1
    local inputFile=$2
    local offset=$3
    dd if="$inputFile" of="$outputFile" bs=1 seek="$offset" conv=notrunc
}

function assembleBinary {
    binSize=$1
    envName=$2
    bootloaderAddress=$(hexToDec $3)
    
    bootloaderPath=".pio/build/$envName/bootloader.bin"
    partitionTablePath=".pio/build/$envName/partitions.bin"
    firmwarePath=".pio/build/$envName/firmware.bin"
    resourcesPath="resources/tools/fs/out/fs.bin"
    
    createEmptyBinary $binSize
    
    bootloaderAddress=$(hexToDec $3)
    partitionTableAddress=$(hexToDec 0x8000)
    firmwareAddress=$(hexToDec 0x10000)
    resourceAddress=$(<resources/tools/fs/in/offset.txt tr -d '\n')
    
    writeAtOffset demo.bin $bootloaderPath $bootloaderAddress
    writeAtOffset demo.bin $partitionTablePath $partitionTableAddress
    writeAtOffset demo.bin $firmwarePath $firmwareAddress
    writeAtOffset demo.bin $resourcesPath $resourceAddress
    
    mv demo.bin resources/tools/other/out/demo/${envName}-demo.bin
}

rm -rf resources/tools/other/out/demo/
mkdir -p resources/tools/other/out/demo/

for env in "${envList[@]}"; do
    if [[ "$env" == "Watchy_1" ]]; then
        assembleBinary 4 Watchy_1 0x1000
        elif [[ "$env" == "Watchy_1_5" ]]; then
        assembleBinary 4 Watchy_1_5 0x1000
        elif [[ "$env" == "Watchy_2" ]]; then
        assembleBinary 4 Watchy_2 0x1000
        elif [[ "$env" == "Watchy_3" ]]; then
        assembleBinary 8 Watchy_3 0x0
        elif [[ "$env" == "Yatchy" ]]; then
        assembleBinary 4 Yatchy 0x0
    fi
done

touch resources/personal/books/.gitkeep
touch resources/personal/vault/.gitkeep

sync
