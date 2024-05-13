#ifndef LITTLEFS_H
#define LITTLEFS_H

#include "defines/defines.h"

bool fsSetup();
void fsCreateDir(String path);

extern String loadedImgNames[IMG_COUNT];
extern ImageDef loadedImg[IMG_COUNT];

ImageDef* getImg(String name);
int16_t getImgWidth(String name);
int16_t getImgHeight(String name);

extern String loadedFontNames[FONT_COUNT];
extern GFXfont loadedFont[FONT_COUNT];

const GFXfont *getFont(String name);

bool fsIsConfig(String conf);
String fsGetString(String conf, String defaultValue);
void fsSetString(String conf, String value);
bufSize fsGetBlob(String conf);
void fsSetBlob(String conf, uint8_t* value, int size);

#if DEBUG
void fsListDir(String dirname, uint8_t levels);
#endif

#endif
