#!/bin/bash

cd ..
git submodule update --init --recursive
if [ ! -f "src/defines/config.h" ] || [ ! -f "src/defines/confidential.h" ]; then
    echo -e ''
    echo "Creating config template"
    cd src/defines/templates/
    cp gifnoc-template.h ../config.h
    cp laitnedifnoc-template.h ../confidential.h
    cd ../../../
fi
cd resources/