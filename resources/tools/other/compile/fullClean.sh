#!/bin/bash

rm -rf .pio/ dependencies.lock sdkconfig.* 
rm -rf .platformio/.cache .platformio/platforms .platformio/packages/framework-e*
rm -rf .esp-idf/esp-idf/*
rm -rf .esp-idf/espressif/*
touch .esp-idf/esp-idf/.gitkeep
touch .esp-idf/espressif/.gitkeep
rm -rf managed_components components
rm -rf resources/tools/other/in/*
rm -rf resources/tools/other/out/*
rm -rf resources/tools/fs/in/*
rm -rf resources/tools/fs/out/*
rm -rf resources/tools/fs/littlefs/*