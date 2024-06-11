#!/bin/bash

rm -rf out/ 1>/dev/null 2>/dev/null
mkdir -p out/conf/
touch out/conf/.keep # They ignore empty dirs...

for file in ../../personal/books/*; do
    if [ ! -d "$file" ] && [[ "$file" =~ \.(epub|pdf)$ ]]; then
        file_name="${file##*/}"
        echo "Processing file: $file_name"
        file_no_ext="${file_name%.*}"
        file_no_ext=$(echo -n $file_no_ext | python3 -c "import sys; from unidecode import unidecode; print(unidecode(sys.stdin.read().strip()))")
        file_no_ext="${file_no_ext// /_}" # Remove spaces to be sure
        mutool convert -F text -o "out/$file_no_ext" "$file"
        sed -i ':a;N;$!ba;s/\n/ /g;s/ \+/ /g' "out/$file_no_ext"
        cat "out/$file_no_ext" | python3 -c "import sys; from unidecode import unidecode; print(unidecode(sys.stdin.read().strip()))" > "out/tmp"
        mv "out/tmp" "out/$file_no_ext"
    fi
done

rm -rf ../fs/littlefs/book 1>/dev/null 2>/dev/null
mv out/ ../fs/littlefs/book/