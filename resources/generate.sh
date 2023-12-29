#!/bin/bash

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
