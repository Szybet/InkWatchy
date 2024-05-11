#ifndef LITTLEFS_H
#define LITTLEFS_H

#include "defines/defines.h"

extern String loadedImgNames[IMG_COUNT];
extern ImageDef loadedImg[IMG_COUNT];

bool setupFsManager();

ImageDef* getImg(String name);
int16_t getImgWidth(String name);
int16_t getImgHeight(String name);

#if DEBUG
void listDir(String dirname, uint8_t levels);
#endif

#endif
