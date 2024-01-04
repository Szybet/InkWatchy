#!/bin/bash

fontconvert="../../.pio/libdeps/InkWatchy/Adafruit GFX Library/fontconvert/fontconvert"
if [ -e "$fontconvert" ]; then
    echo "fontconvert exists"
else
    echo "fontconvert does not exist, creating it..."
    cd ../../.pio/libdeps/InkWatchy/Adafruit\ GFX\ Library/fontconvert/
    make -j$(nproc)
    cd ../../../../../resources/fonts/
    if [ ! -e "$fontconvert" ]; then
        echo "Coudln't compile fontconvert, exiting"
        exit 1
    fi
fi

rm fonts.h 1>/dev/null 2>/dev/null
touch fonts.h

echo -e "#ifndef FONTS_H" >> fonts.h
echo -e "#define FONTS_H" >> fonts.h
echo -e '' >> fonts.h

for d in *
do
    if [[ $d == *".sh"* ]] || [[ $d == *".h"* ]]; then
        continue
    fi

    if [ -f "$d" ]; then
        continue
    fi

    echo "Processing directory $d"
    for f in "$d"/*
    do
        if [[ $f != *".ttf"* ]]; then
            continue
        fi
        echo "Processing file $f"
        ./"$fontconvert" $f $d | sed 's/PROGMEM //g'>> fonts.h
        echo -e '' >> fonts.h
    done
done

echo "#endif" >> fonts.h

mv fonts.h ../../src/defines/
