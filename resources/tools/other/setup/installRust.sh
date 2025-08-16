#!/bin/bash

# it's here because of mount points in devcontainer

export PATH="$HOME/.cargo/bin:$PATH"

if ! command -v cargo &> /dev/null || \
   ! command -v rustup &> /dev/null; then
    echo "Installing Rust..."
    curl https://sh.rustup.rs -sSf | sh -s -- --default-toolchain stable -y
fi

source $HOME/.cargo/env

if [ ! -d "$HOME/.rustup/toolchains/esp" ] || [ ! -d "$HOME/.espup/esp-clang" ] || ! command -v espup &> /dev/null; then
    echo "Installing ESP tools..."
    $HOME/.cargo/bin/cargo install espup --locked --force
    $HOME/.cargo/bin/espup install
fi

if ! command -v cbindgen &> /dev/null; then
    echo "Installing cbindgen..."
    $HOME/.cargo/bin/cargo install --force cbindgen
fi