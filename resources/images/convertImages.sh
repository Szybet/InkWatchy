#!/bin/bash
source $(dirname "$0")/../xxd_legacy.sh


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
    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- |  xxd -i | xxd_wrapper $fnel $fne >>images.h

    echo -n -e "const ImageDef ${fnel}Pack = {${fnel}, ${fne}_WIDTH, ${fne}_HEIGHT};\n" >>images.h 



     #xxd_img_legacy $f $fnel $fne  images.h

done

echo "#endif" >> images.h

mv images.h ../../src/defines/
