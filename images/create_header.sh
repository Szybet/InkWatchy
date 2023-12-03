#!/bin/bash

rm images.h 1>/dev/null 2>/dev/null
touch images.h

echo -e "#ifndef IMAGES_H" >> images.h
echo -e "#define IMAGES_H" >> images.h
echo -e '' >> images.h

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]]; then
        continue
    fi

    fne="${f%.*}Img"

    echo "Processing $f" # always double quote "$f" filename

    convert $f -colorspace gray h:- | sed 's/MagickImage/'$fne'/g' | tail -n +4 >> images.h
    echo -e '' >> images.h

done

echo "#endif" >> images.h

mv images.h ../src/defines/
