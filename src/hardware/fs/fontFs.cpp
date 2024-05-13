#include "littlefs.h"

String loadedFontNames[FONT_COUNT];
GFXfont loadedFont[FONT_COUNT];

const GFXfont *getFont(String name)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return &FreeSansBold9pt7b;
    }
    uint8_t emptyListIndex = 0;
    for (int i = 0; i < IMG_COUNT; i++)
    {
        if (name == loadedFontNames[i])
        {
            return &loadedFont[i];
        }
        else
        {
            if (loadedFontNames[i] == "")
            {
                emptyListIndex = i;
                break;
            }
        }
    }
    // Bitmap
    File fileBitmap = LittleFS.open("/font/" + name + "Bitmap");
    if (fileBitmap == false)
    {
        debugLog("File is not available: " + name + "Bitmap");
        return &FreeSansBold9pt7b;
    }
    int fileBitmapSize = fileBitmap.size();
    debugLog("file size: " + String(fileBitmapSize));
    if (fileBitmapSize <= 0)
    {
        debugLog("This file has size 0: " + name + "Bitmap");
    }
    uint8_t *bitmapBuf = (uint8_t *)malloc(fileBitmapSize * sizeof(uint8_t));
    if (fileBitmap.read(bitmapBuf, fileBitmapSize) < 0)
    {
        debugLog("Failed to read the file: " + name + "Bitmap");
        return &FreeSansBold9pt7b;
    }
    fileBitmap.close();
    // Glyph
    File fileGlyph = LittleFS.open("/font/" + name + "Glyphs");
    if (fileGlyph == false)
    {
        debugLog("File is not available: " + name + "Glyphs");
        return &FreeSansBold9pt7b;
    }
    int fileGlyphSize = fileGlyph.size();
    if(fileGlyphSize != 95 * sizeof(GFXglyph)) {
        debugLog("Glyphs are fucked up :(");
    }
    uint8_t *glyphBuf = (uint8_t *)malloc(95 * sizeof(GFXglyph));
    if (fileGlyph.read(glyphBuf, fileGlyphSize) < 0)
    {
        debugLog("Failed to read the file: " + name + "Glyphs");
        return &FreeSansBold9pt7b;
    }
    GFXglyph* glyphs = reinterpret_cast<GFXglyph*>(glyphBuf);
    fileGlyph.close();
    // Struct
    File fileStruct = LittleFS.open("/font/" + name + "Struct");
    if (fileStruct == false)
    {
        debugLog("File is not available: " + name + "Struct");
        return &FreeSansBold9pt7b;
    }
    int fileStructSize = fileStruct.size();
    debugLog("fileStructSize: " + String(fileStructSize));
    uint8_t *structBuf = (uint8_t *)malloc(fileStructSize * sizeof(uint8_t));
    if (fileStruct.read(structBuf, fileStructSize) < 0)
    {
        debugLog("Failed to read the file: " + name + "Struct");
        return &FreeSansBold9pt7b;
    }
    fileStruct.close();
    uint16_t first = ((uint16_t)structBuf[1] << 8) | structBuf[0];
    uint16_t last = ((uint16_t)structBuf[3] << 8) | structBuf[2];
    uint8_t yAdvance = structBuf[4];
    GFXfont newFont = GFXfont {
        bitmapBuf,
        glyphs,
        first,
        last,
        yAdvance,
    };
    loadedFont[emptyListIndex] = newFont;
    loadedFontNames[emptyListIndex] = name;
    return &loadedFont[emptyListIndex];
}