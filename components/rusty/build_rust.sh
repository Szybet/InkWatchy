#!/bin/bash

# Add to .vscode/settings.json
#    "rust-analyzer.linkedProjects": [
#        "${workspaceFolder}/components/rusty/inkrusty/Cargo.toml",
#        "${workspaceFolder}/components/rusty/crates/general_page/Cargo.toml",
#    ],
#     "rust-analyzer.cargo.target": "riscv32imac-unknown-none-elf",

source resources/tools/globalFunctions.sh
pio_env=$(get_pio_env .vscode/launch.json)

resources/tools/other/setup/installRust.sh

base_features=""

check_features() {
    local config_path=$1
    shift
    base_features=""
    
    for feature in "$@"; do
        # echo $feature
        # Check both uppercase and lowercase versions
        local upper_feature=$(echo "$feature" | tr '[:lower:]' '[:upper:]')
        local lower_feature=$(echo "$feature" | tr '[:upper:]' '[:lower:]')
        
        # Search for uppercase define in config
        local value=$(grep -E "^\s*#define\s+($upper_feature|$lower_feature)\s+" "$config_path" | 
                      awk '{print $3}' | tr -d '"' | head -1)
        
        if [ "$value" = "1" ]; then
            base_features="$base_features,$feature"
        fi
    done
}

check_features "src/defines/config.h" debug snake ink_alarms set_clock_gui

# A bit of condition.h logic
has_ink=false
has_clock=false

if [[ $base_features == *"ink_alarms"* ]]; then
  base_features=${base_features//",ink_alarms"/}
  has_ink=true
fi

if [[ $base_features == *"set_clock_gui"* ]]; then
  base_features=${base_features//",set_clock_gui"/}
  has_clock=true
fi

if $has_ink || $has_clock; then
  base_features="$base_features,set_time"
fi

# Done
base_features="${base_features:1}"

echo "Features are: $base_features"

cd components/rusty

rm -rf lib include
mkdir lib include

echo "Pio env in build rust is: $pio_env"

target=""
if [[ "$pio_env" == "Yatchy" ]]; then
    echo "Rust: Yatchy"
    target="riscv32imac-unknown-none-elf"
elif [[ "$pio_env" == "Watchy_3" ]]; then
    echo "Rust: Watchy 3"
    target="xtensa-esp32s3-none-elf"
else
    echo "Rust: Watchy 2 and friends"
    target="xtensa-esp32-none-elf"
fi

cd inkrusty/
cargo build --target "$target" --release --features=$base_features || {
    echo "Build failed for target: $target" >&2
    exit 1
}
cp target/$target/release/libinkrusty.a ../lib/librusty.a
cbindgen . --quiet --config cbindgen.toml --lang c -o ../include/rusty.h

du -h ../lib/librusty.a
du ../lib/librusty.a