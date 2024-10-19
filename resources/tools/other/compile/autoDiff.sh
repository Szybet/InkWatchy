#!/bin/bash

check_second_word() {
    local word="$1"
    local file="$2"

    while IFS= read -r line; do
        second_word=$(echo "$line" | awk '{print $2}')
        
        if [[ "$second_word" == *"$word"* ]]; then
            #echo "Found ${word}" >&2
            return 0
        fi
    done < "$file"

    echo "Not found: ${word}" >&2
    return 1
}

update_config_file() {
    local sour_file="$1"
    local dest_file="$2"

    mkdir -p resources/personal/oldConfigs/
    cp "$dest_file" resources/personal/oldConfigs/ 

    local line_num=1
    while read -r line; do
        save_line=$line
        second_word=$(echo "$save_line" | awk '{print $2}')
        echo "Checking for: ${second_word}"
        if ! check_second_word "$second_word" "$dest_file"; then
            echo "Adding line: ${save_line}" 
            line_count=$(wc -l < $dest_file)
            if [ "$line_count" -lt "$line_num" ]; then
                echo "Line count ($line_count) is smaller than line number ($line_num)."
                sed -i '$d' "$dest_file"
                echo "${save_line}" >> "$dest_file"
                echo "#endif" >> "$dest_file"
            else
                # echo "Line count ($line_count) is not smaller than line number ($line_num)."
                echo "Adding at ${line_num} line: ${save_line}"
                sed -i "${line_num}i ${save_line}" "$dest_file"
            fi
        fi
        ((line_num++))
    done < "$sour_file"
}

update_config_file "src/defines/templates/gifnoc-template.h" "src/defines/config.h"
update_config_file "src/defines/templates/laitnedifnoc-template.h" "src/defines/confidential.h"