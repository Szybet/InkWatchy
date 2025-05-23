#!/bin/bash

config_file="src/defines/confidential.h"
calendar_dir="resources/tools/fs/littlefs/calendar"
mapfile -t urls < <(grep '^[[:space:]]*#define CALENDAR_URL' "$config_file" | awk '{print $3}' | tr -d '"')

echo "Found URLs:"
for u in "${urls[@]}"; do
    echo "$u"
done

mkdir -p $calendar_dir/
rm -rf $calendar_dir/*

for url in "${urls[@]}"; do
    echo "Processing URL: $url"
    cargo run --manifest-path src/ui/places/calendar/ics-to-json-rs/Cargo.toml -- -u "$url" -o $calendar_dir/
done

merged_file="$calendar_dir/index.txt"

cat "$calendar_dir"/index.txt* | sort -n | uniq > "$calendar_dir/index.txt.tmp" && mv "$calendar_dir/index.txt.tmp" "$merged_file"

find "$calendar_dir" -type f -name "index.txt-*" -delete

cd resources/tools/fs
./getFs.sh
rm -rf out/fsDump/calendar/
mkdir -p out/fsDump/calendar/
cp -r littlefs/calendar/* out/fsDump/calendar/
./flashPersonalFs.sh

