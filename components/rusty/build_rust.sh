#!/bin/bash

# Add to .vscode/settings.json
#    "rust-analyzer.linkedProjects": [
#        "${workspaceFolder}/components/rusty/inkrusty/Cargo.toml",
#    ],

source resources/tools/globalFunctions.sh
pio_env=$(get_pio_env .vscode/launch.json)

resources/tools/other/setup/installRust.sh

check_features() {
    local config_path=$1
    shift
    base_features="none"
    
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

    echo "Features are: $base_features"
}

check_features "src/defines/config.h" debug snake

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
cargo build --target "$target" --release --features $base_features || {
    echo "Build failed for target: $target" >&2
    exit 1
}
cp target/$target/release/libinkrusty.a ../lib/librusty.a
cbindgen . --quiet --config cbindgen.toml --lang c -o ../include/rusty.h
