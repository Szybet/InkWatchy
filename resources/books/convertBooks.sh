#!/bin/bash

rm -rf out/
mkdir -p out/

for file in *; do
    if [ ! -d "$file" ] && [[ "$file" =~ \.(epub|pdf)$ ]]; then
        echo "Processing file: $file"
        file_no_ext="${file%.*}"
        mutool convert -F text -o "out/$file_no_ext" "$file"
        sed -i ':a;N;$!ba;s/\n/ /g;s/ \+/ /g' "out/$file_no_ext"
    fi
done

rm -rf ../fs/littlefs/book 1>/dev/null 2>/dev/null
cp -r out/ ../fs/littlefs/book