#!/bin/bash

git submodule update --init --recursive

./other/generateConfig.sh

echo -e ''
./other/packages.sh

echo "Patching some libraries so you won't see warnings :D"
./other/patchLibs.sh

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
./convertBooks.sh
cd ../

echo -e ''
echo "Processing vault"
cd vault/
./convertImagesVault.sh
cd ../