#!/bin/bash

config_file="../../../../src/defines/config.h"
vault_password=$(grep '#define VIDEO_PLAYER' "$config_file" | awk '{print $3}' | tr -d '"')

if [[ $vault_password == 0* ]]; then
    echo "Video player is disabled"
    exit
fi

rm -rf out/ 1>/dev/null 2>/dev/null
mkdir -p out/

for file in ../../../personal/videos/*; do
    if [ ! -d "$file" ] && [[ "$file" =~ \.(mp4)$ ]]; then
        file_name="${file##*/}"
        echo "Processing file: $file_name"
        echo "At $file"
        file_no_ext="${file_name%.*}"
        fps=$(( $(ffprobe -v 0 -of csv=p=0 -select_streams v:0 -show_entries stream=r_frame_rate "$file" | sed 's#/# / #g') ))
        fps=$(( fps / 2 ))
        if [ "$fps" -eq 0 ]; then
            fps=1
        fi
        echo "Fps falf: $fps"
        mkdir "out/$file_no_ext"
        ffmpeg -loglevel quiet -i $file -vf "select=not(mod(n\,$fps)),scale=200:200" -vsync vfr out/$file_no_ext/%d.png 

        for png_file in out/$file_no_ext/*; do
            png_file_name="${png_file##*/}"
            png_file_no_ext="${png_file_name%.*}"
            echo "Frame: $png_file"
            ../../other/in/magick $png_file -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../../images/img/eink-2color.png -depth 1 gray:- > out/$file_no_ext/$png_file_no_ext
        done
        rm -rf out/$file_no_ext/*.png
    fi
done

rm -rf ../../fs/littlefs/videos 1>/dev/null 2>/dev/null
mv out/ ../../fs/littlefs/videos/