#!/bin/bash
source ../globalFunctions.sh

pio_env=$(get_pio_env ../../../.vscode/launch.json)

fontconvert="../../../.pio/libdeps/$pio_env/Adafruit GFX Library/fontconvert/fontconvert"
if [ -e "$fontconvert" ]; then
    echo "fontconvert exists"
else
    echo "fontconvert does not exist, creating it..."
    cd ../../../.pio/libdeps/$pio_env/Adafruit\ GFX\ Library/fontconvert/
    make -j$(nproc)
    cd ../../../../../resources/tools/fonts/
    if [ ! -e "$fontconvert" ]; then
        echo "Coudln't compile fontconvert, exiting"
        exit 1
    fi
fi

rm -rf out/ 1>/dev/null 2>/dev/null
mkdir -p out/

for d in fnt/*
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

    font_size="${d##*/}"
    echo "Processing fonts of size $font_size"
    for f in "$d"/*
    do
        if [[ $f != *".ttf"* ]]; then
            continue
        fi
        f_name="${f##*/}"
        echo "Processing file $f_name"
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
        ./"$fontconvert" out/font.ttf $font_size | sed 's/PROGMEM //g'>> out/fonts.h
        # Remove comments
        sed -i '/^\/\//d;/^\/\*$/,/^\*\//d' out/fonts.h
        sed -i 's,//.*$,,' out/fonts.h
        
        # Remove font sizes
        sed -i "s/${font_size}pt.*b//g" out/fonts.h

        count=$(grep -o '0x' out/fonts.h | wc -l)
        count=$((count - 2))

        echo "#define BYTE_COUNT $count" >> out/fonts.h
        # stat -c "%s" out/fontStruct.bin
        rm out/font.bin 1>/dev/null 2>/dev/null
        rm out/fontDumper 1>/dev/null 2>/dev/null
        g++ -I out/ fontDumper.cpp -o out/fontDumper
        out/fontDumper

        pure_name="${f_name%.ttf}${font_size}"

        mv out/font.bin out/$pure_name
        rm out/fontDumper 1>/dev/null 2>/dev/null
    done
done

rm -f out/font.ttf out/fonts.h out/fontDumper
rm -rf ../fs/littlefs/font/ 1>/dev/null 2>/dev/null
mv out ../fs/littlefs/font/