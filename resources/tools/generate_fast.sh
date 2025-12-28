#!/bin/bash

echo -e ''
echo "Processing images"
cd images/
./convertImages.sh &
cd ../

echo -e ''
echo "Processing fonts"
cd fonts/
./convertFonts.sh &
cd ../

echo -e ''
echo "Processing book"
cd books/
./convertBooks.sh &
cd ../

echo -e ''
echo "Processing vault"
cd vault/
./convertImagesVault.sh &
cd ../

echo -e ''
echo "Processing videos"
cd other/videos/
./convertVideos.sh &
cd ../../

wait
