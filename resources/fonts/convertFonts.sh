#!/bin/bash
source ../global_functions.sh

pio_env=$(get_pio_env)

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

words=$(grep -oP '(?<=GFXfont\s)\w+' fonts.h)
font_count=$(grep -oP '(?<=GFXfont\s)\w+' fonts.h | wc -l)

formatted_fonts_ref=""
for word in $words; do
    formatted_fonts_ref+="  $word,\n"
done

formatted_fonts_str=""
for word in $words; do
    formatted_fonts_str+="  \"$word\",\n"
done


echo -e "#if FONT_MENU_ENABLED" >> fonts.h
echo -e "#define FONT_COUNT $font_count" >> fonts.h
# Sadly I couldn't do references for some reason so we are stuck with this
echo -e "const GFXfont fontListRef[FONT_COUNT] = {" >> fonts.h
echo -e "$formatted_fonts_ref};" >> fonts.h

echo -e '' >> fonts.h

echo -e "const String fontListStr[FONT_COUNT] = {" >> fonts.h
echo -e "$formatted_fonts_str};" >> fonts.h
echo -e "#endif" >> fonts.h

echo -e '' >> fonts.h

echo "#endif" >> fonts.h

mv fonts.h ../../src/defines/
