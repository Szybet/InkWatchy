#!/bin/bash

# it's here because of mount points in devcontainer

if ! command -v ~/.cargo/bin/cargo &> /dev/null || \
   ! command -v ~/.cargo/bin/rustup &> /dev/null || \
   ! command -v ~/.cargo/bin/espup &> /dev/null || \
   ! command -v ~/.cargo/bin/cbindgen &> /dev/null; then
       
    echo "Installing Rust..."
    curl https://sh.rustup.rs -sSf | sh -s -- --default-toolchain stable -y
    
    source ~/.cargo/env
    export PATH="$HOME/.cargo/bin:$PATH"

    echo "Installing ESP tools..."
    ~/.cargo/bin/cargo install espup --locked
    ~/.cargo/bin/espup install
    ~/.cargo/bin/cargo install --force cbindgen
else
    echo "Rust is already installed, skipping installation."
fi
