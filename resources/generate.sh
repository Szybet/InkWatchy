#!/bin/bash

echo "Patching some libraries so you won't see warnings :D"
./patchLibs.sh

echo -e ''
echo "Processing images"
cd images/
./convertImages.sh
cd ../

echo -e ''
echo "Processing fonts"
cd fonts/
./convertFonts.sh
cd ../

echo -e ''
echo "Processing book"
cd books/
./convertBooks.py
cd ../

echo -e ''
echo "Processing vault"
cd vault/
./convertImagesVault.sh
cd ../

cd ../
if [ ! -f "src/defines/config.h" ] || [ ! -f "src/defines/confidential.h" ]; then
    echo -e ''
    echo "Creating config template"
    cd resources
    ./generate_config.sh
    cd ../
fi

echo -e ''
cd resources/other
./packages.sh
cd ../../