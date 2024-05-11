#!/bin/bash
source ../global_functions.sh

pio_env=$(get_pio_env ../../.vscode/launch.json)

fontconvert="../../.pio/libdeps/$pio_env/Adafruit GFX Library/fontconvert/fontconvert"
if [ -e "$fontconvert" ]; then
    echo "fontconvert exists"
else
    echo "fontconvert does not exist, creating it..."
    cd ../../.pio/libdeps/$pio_env/Adafruit\ GFX\ Library/fontconvert/
    make -j$(nproc)
    cd ../../../../../resources/fonts/
    if [ ! -e "$fontconvert" ]; then
        echo "Coudln't compile fontconvert, exiting"
        exit 1
    fi
fi

mkdir -p out/

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
        rm fonts.h 1>/dev/null 2>/dev/null
        touch fonts.h

        echo "#include <cstdint>" > fonts.h
        echo 'typedef struct {' >> fonts.h
        echo '  uint16_t bitmapOffset;' >> fonts.h
        echo '  uint8_t width;' >> fonts.h
        echo '  uint8_t height;' >> fonts.h
        echo '  uint8_t xAdvance;' >> fonts.h
        echo '  int8_t xOffset;' >> fonts.h
        echo '  int8_t yOffset;' >> fonts.h
        echo '} GFXglyph;' >> fonts.h

        echo 'typedef struct {' >> fonts.h
        echo '  uint8_t *bitmap;' >> fonts.h
        echo '  GFXglyph *glyph;' >> fonts.h
        echo '  uint16_t first;' >> fonts.h
        echo '  uint16_t last;' >> fonts.h
        echo '  uint8_t yAdvance;' >> fonts.h
        echo '} GFXfont;' >> fonts.h


        cp $f font.ttf
        ./"$fontconvert" font.ttf $d | sed 's/PROGMEM //g'>> fonts.h

        # Remove comments
        sed -i '/^\/\//d;/^\/\*$/,/^\*\//d' fonts.h
        sed -i 's,//.*$,,' fonts.h

        # Remove font sizes
        sed -i "s/${d}pt.*b//g" fonts.h

        count=$(grep -o '0x' fonts.h | wc -l)
        count=$((count - 2))

        echo "#define BYTE_COUNT $count" >> fonts.h

        g++ fontDumper.cpp -o out/fontDumper
        # stat -c "%s" out/fontStruct.bin
        rm out/fontBitmaps.bin 1>/dev/null 2>/dev/null
        rm out/fontGlyphs.bin 1>/dev/null 2>/dev/null
        rm out/fontStruct.bin 1>/dev/null 2>/dev/null
        out/fontDumper

    done
done