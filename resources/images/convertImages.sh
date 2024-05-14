#!/bin/bash
source ../global_functions.sh

rm -rf img 1>/dev/null 2>/dev/null
mkdir -p img

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f"
    
    img_name="${f%.*}"
    img_path="img/${img_name}"
    img_info="img/${img_name}Inf"

    identify -ping -format '%w\n%h' $f > ${img_info}

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- > ${img_path}

done

rm -rf ../fs/littlefs/img/
mv img ../fs/littlefs/