#pragma once

#include "defines.h"
// https://github.com/espressif/arduino-esp32/blob/master/libraries/LittleFS/examples/LITTLEFS_test/LITTLEFS_test.ino

bool fsSetup();
void fsCreateDir(String path);
int fsItemsInDir(String dir);
void fsAppendToFile(String path, String str);
size_t fsGetFileSize(String path);
void fsRemoveFile(String path);
bool fsFileExists(String path);
bool removeDir(String path);

extern char loadedImgNames[IMG_COUNT][RESOURCES_NAME_LENGTH];
extern ImageDef loadedImg[IMG_COUNT];

ImageDef* getImg(String name);
int16_t getImgWidth(String name);
int16_t getImgHeight(String name);

extern char loadedFontNames[FONT_COUNT][RESOURCES_NAME_LENGTH];
extern GFXfont loadedFont[FONT_COUNT];

const GFXfont* getFont(String name);

// bool fsIsConfig(String conf, String dir = "/conf/");
String fsGetString(String conf, String defaultValue, String dir = "/conf/");
bool fsSetString(String conf, String value, String dir = "/conf/");
bufSize fsGetBlob(String conf, String dir = "/conf/");
bool fsSetBlob(String conf, uint8_t* value, int size, String dir = "/conf/");

void cleanFontCache();
void cleanImgCache();

#if DEBUG
void fsListDir(String dirname, uint8_t levels);
#endif

