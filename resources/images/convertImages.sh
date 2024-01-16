#!/bin/bash

rm images.h 1>/dev/null 2>/dev/null
touch images.h

echo -e "#ifndef IMAGES_H" >> images.h
echo -e "#define IMAGES_H" >> images.h
echo -e '' >> images.h

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f" # always double quote "$f" filename
    
    fnel="${f%.*}Img"
    fne=$(echo ${f%.*}_IMG | tr a-z A-Z)

    #convert $f -compress none -monochrome -colors 2 -depth 1 h:- | sed 's/MagickImage/'$fne'/g' | tail -n +4 >> images.h

    #echo -e "#define ${fne}Pack ${fne},${fne}Width,${fne}Height" >> images.h

    identify -ping -format '#define '${fne}'_WIDTH %w' $f >> images.h
    echo -e '' >> images.h

    identify -ping -format '#define '${fne}'_HEIGHT %h' $f >> images.h
    echo -e '' >> images.h

    # On arch linux install xxd-standalone

    echo -e -n "const unsigned char " >>images.h
    echo -n "${fnel}" >>images.h
    echo -e -n "[] = {\n" >>images.h

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- |  xxd -i  >> images.h

    echo -e "};\n" >> images.h
    echo -n "const unsigned int " >>images.h
    echo -n -e "${fnel}" >> images.h
    echo -n "d = " >>images.h
    echo -n "(sizeof(" >>images.h
    echo -n -e "$fnel" >>images.h
    echo -n ")/sizeof(" >>images.h
    echo -n -e "${fnel}" >>images.h
    echo -n -e "[0])) ;\n" >>images.h

    echo -n -e "const ImageDef ${fnel}Pack = {${fnel}, ${fne}_WIDTH, ${fne}_HEIGHT};" >> images.h
    echo -e '' >> images.h

done

echo "#endif" >> images.h

mv images.h ../../src/defines/
