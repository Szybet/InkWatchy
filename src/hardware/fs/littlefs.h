#ifndef LITTLEFS_H
#define LITTLEFS_H

#include "defines/defines.h"

void setupFsManager();

#if DEBUG
void listDir(String dirname, uint8_t levels);
#endif

#endif
