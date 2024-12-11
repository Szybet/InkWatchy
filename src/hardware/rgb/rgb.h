#ifndef RGB_H
#define RGB_H

#include "defines.h"

extern bool rgbTaskRunning;
extern std::mutex rgbTaskMutex;

typedef enum
{
    IwNone = 0,
    IwRed = 1,
    IwGreen = 2,
    IwBlue = 3,
    IwYellow = 4,
    IwPink = 5,
    IwCyan = 6,
    IwWhite = 7,
} IWColors; // IW as inkwatchy

void setRgb(IWColors color, bool clearPrevious = true, uint timeMs = 0);

#endif