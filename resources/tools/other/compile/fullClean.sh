#!/bin/bash

rm -rf .root/.cargo/bin/cargo # To restart rust installation
rm -rf .pio/ dependencies.lock sdkconfig.* 
rm -rf .root/.platformio/.cache .root/.platformio/platforms .root/.platformio/packages/framework-e*
# rm -rf .root/.platformio/packages/tl-install # Fixes softlock if bad install happens, or does it?
rm -rf .root/.esp-idf/esp-idf/*
rm -rf .root/.esp-idf/espressif/*
touch .root/.esp-idf/espressif/.gitkeep
rm -rf managed_components
rm -rf resources/tools/other/in/*
rm -rf resources/tools/other/out/*
rm -rf resources/tools/fs/in/*
rm -rf resources/tools/fs/out/*
rm -rf resources/tools/fs/littlefs/*
echo "Waiting for 60 seconds for platformio to shut up, You should click the cancel button yourself too"
sleep 60