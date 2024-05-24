#!/bin/bash
source ../globalFunctions.sh

rm -rf out 1>/dev/null 2>/dev/null
mkdir -p out

for f in img/*
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f"
    
    img_name="${f##*/}"
    img_name="${img_name%.*}"
    img_path="out/${img_name}"
    img_info="out/${img_name}Inf"

    identify -ping -format '%w\n%h' $f > ${img_info}

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap img/eink-2color.png -depth 1 gray:- > ${img_path}
done

rm -rf ../fs/littlefs/img/ 1>/dev/null 2>/dev/null
mv out ../fs/littlefs/img/