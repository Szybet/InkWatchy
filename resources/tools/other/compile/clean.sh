#!/bin/bash

# I don't clean .pio/libdeps here
rm -rf .pio/build/ dependencies.lock sdkconfig.* 
# rm -rf managed_components
# rm -rf resources/tools/other/in/*
# rm -rf resources/tools/other/out/*
# rm -rf resources/tools/fs/in/*
# rm -rf resources/tools/fs/out/*
# rm -rf resources/tools/fs/littlefs/*