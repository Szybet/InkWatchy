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
rm -rf ../fs/littlefs/font/
mkdir -p ../fs/littlefs/font/

for d in *
do
    if [[ $d == *".sh"* ]] || [[ $d == *".h"* ]]; then
        continue
    fi

    if [ -f "$d" ]; then
        continue
    fi

    if [ $d == "out" ]; then
        continue
    fi

    echo "Processing directory $d"
    for f in "$d"/*
    do
        if [[ $f != *".ttf"* ]]; then
            continue
        fi
        echo "Processing file $f"
        rm out/fonts.h 1>/dev/null 2>/dev/null
        touch out/fonts.h

        echo "#include <cstdint>" >> out/fonts.h
        echo 'typedef struct {' >> out/fonts.h
        echo '  uint16_t bitmapOffset;' >> out/fonts.h
        echo '  uint8_t width;' >> out/fonts.h
        echo '  uint8_t height;' >> out/fonts.h
        echo '  uint8_t xAdvance;' >> out/fonts.h
        echo '  int8_t xOffset;' >> out/fonts.h
        echo '  int8_t yOffset;' >> out/fonts.h
        echo '} GFXglyph;' >> out/fonts.h

        echo 'typedef struct {' >> out/fonts.h
        echo '  uint8_t *bitmap;' >> out/fonts.h
        echo '  GFXglyph *glyph;' >> out/fonts.h
        echo '  uint16_t first;' >> out/fonts.h
        echo '  uint16_t last;' >> out/fonts.h
        echo '  uint8_t yAdvance;' >> out/fonts.h
        echo '} GFXfont;' >> out/fonts.h

        rm out/font.ttf 1>/dev/null 2>/dev/null
        cp $f out/font.ttf
        ./"$fontconvert" out/font.ttf $d | sed 's/PROGMEM //g'>> out/fonts.h

        # Remove comments
        sed -i '/^\/\//d;/^\/\*$/,/^\*\//d' out/fonts.h
        sed -i 's,//.*$,,' out/fonts.h

        # Remove font sizes
        sed -i "s/${d}pt.*b//g" out/fonts.h

        count=$(grep -o '0x' out/fonts.h | wc -l)
        count=$((count - 2))

        echo "#define BYTE_COUNT $count" >> out/fonts.h
        # stat -c "%s" out/fontStruct.bin
        rm out/fontBitmaps.bin 1>/dev/null 2>/dev/null
        rm out/fontGlyphs.bin 1>/dev/null 2>/dev/null
        rm out/fontStruct.bin 1>/dev/null 2>/dev/null
        rm out/fontDumper 1>/dev/null 2>/dev/null
        g++ -I out/ fontDumper.cpp -o out/fontDumper
        out/fontDumper

        pure_name="${f/$d\/}"
        pure_name="${pure_name/".ttf"}"
        mv out/fontBitmaps.bin ../fs/littlefs/font/${pure_name}${d}Bitmap
        mv out/fontGlyphs.bin ../fs/littlefs/font/${pure_name}${d}Glyphs
        mv out/fontStruct.bin ../fs/littlefs/font/${pure_name}${d}Struct
    done
done
rm -f out/*