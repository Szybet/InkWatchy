#!/bin/bash

rm images.h 1>/dev/null 2>/dev/null
touch images.h

echo -e "#ifndef IMAGES_H" >> images.h
echo -e "#define IMAGES_H" >> images.h
echo -e '' >> images.h

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]]; then
        continue
    fi

    echo "Processing $f" # always double quote "$f" filename
    
    fne="${f%.*}Img"

    #convert $f -compress none -monochrome -colors 2 -depth 1 h:- | sed 's/MagickImage/'$fne'/g' | tail -n +4 >> images.h

    echo -e "#define ${fne}Pack ${fne},${fne}Width,${fne}Height" >> images.h

    identify -ping -format '#define '${fne}'Width %w' $f >> images.h
    echo -e '' >> images.h

    identify -ping -format '#define '${fne}'Height %h' $f >> images.h
    echo -e '' >> images.h

    # On arch linux install xxd-standalone

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- | xxd -i -n $fne | sed 's/unsigned/const unsigned/g' | sed '/_len = /d' >> images.h
    echo -e '' >> images.h

done

echo "#endif" >> images.h

mv images.h ../src/defines/
