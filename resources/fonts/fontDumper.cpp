#include "fonts.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

void dumpBitmaps() {
    std::ofstream outFile("out/fontBitmaps.bin", std::ios::binary);

    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < BYTE_COUNT; ++i) {
        outFile.put(fontBitmaps[i]);
    }

    if (!outFile.good()) {
        std::cerr << "Error occurred while writing to file." << std::endl;
        exit(1);
    }

    outFile.close();
}

void dumpGlyphs() {
    FILE* file = fopen("out/fontGlyphs.bin", "wb");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (size_t i = 0; i < 95; ++i) {
        fwrite(&fontGlyphs[i], sizeof(GFXglyph), 1, file);
    }

    fclose(file);
}

void dumpFontStruct() {
    FILE* file = fopen("out/fontStruct.bin", "wb");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fwrite(&font.first, sizeof(uint16_t), 1, file);
    fwrite(&font.last, sizeof(uint16_t), 1, file);
    fwrite(&font.yAdvance, sizeof(uint8_t), 1, file);

    fclose(file);
}

int main() {
    std::cout << "Dumping..." << std::endl;

    dumpBitmaps();
    dumpGlyphs();
    dumpFontStruct();

    exit(0);
}