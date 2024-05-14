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

bool fsIsConfig(String conf, String dir = "/conf/");
String fsGetString(String conf, String defaultValue, String dir = "/conf/");
void fsSetString(String conf, String value, String dir = "/conf/");
bufSize fsGetBlob(String conf, String dir = "/conf/");
void fsSetBlob(String conf, uint8_t* value, int size, String dir = "/conf/");

#if DEBUG
void fsListDir(String dirname, uint8_t levels);
#endif

#endif
