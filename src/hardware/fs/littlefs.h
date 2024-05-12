#ifndef LITTLEFS_H
#define LITTLEFS_H

#include "defines/defines.h"

bool setupFsManager();

extern String loadedImgNames[IMG_COUNT];
extern ImageDef loadedImg[IMG_COUNT];

ImageDef* getImg(String name);
int16_t getImgWidth(String name);
int16_t getImgHeight(String name);

extern String loadedFontNames[FONT_COUNT];
extern GFXfont loadedFont[FONT_COUNT];

const GFXfont *getFont(String name);

#if DEBUG
void listDir(String dirname, uint8_t levels);
#endif

#endif
