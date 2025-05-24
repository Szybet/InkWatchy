#!/bin/bash

# it's here because of mount points in devcontainer

if ! command -v /root/.cargo/bin/cargo &> /dev/null || \
   ! command -v /root/.cargo/bin/rustup &> /dev/null || \
   ! command -v /root/.cargo/bin/espup &> /dev/null || \
   ! command -v /root/.cargo/bin/cbindgen &> /dev/null; then
       
    echo "Installing Rust..."
    curl https://sh.rustup.rs -sSf | sh -s -- --default-toolchain stable -y
    
    source /root/.cargo/env
    export PATH="$HOME/.cargo/bin:$PATH"

    echo "Installing ESP tools..."
    /root/.cargo/bin/cargo install espup --locked
    /root/.cargo/bin/espup install
    /root/.cargo/bin/cargo install --force cbindgen
else
    echo "Rust is already installed, skipping installation."
fi
