#!/bin/bash
# Launched from the main dir

config_file="src/defines/confidential.h"
url=$(grep '#define CALENDAR_URL' "$config_file" | awk '{print $3}' | tr -d '"')

echo "Got the url:"
echo $url

mkdir -p resources/tools/fs/littlefs/calendar/
rm -rf resources/tools/fs/littlefs/calendar/*
cargo run --manifest-path src/ui/calendar/ics-to-json-rs/Cargo.toml -- -u $url -o resources/tools/fs/littlefs/calendar/

cd resources/tools/fs
./getFs.sh
rm -rf out/fsDump/calendar/
mkdir -p out/fsDump/calendar/
cp -r littlefs/calendar/* out/fsDump/calendar/
./flashPersonalFs.sh