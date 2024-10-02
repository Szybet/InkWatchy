#ifndef RGB_H
#define RGB_H

#include "defines/defines.h"

extern bool rgbTaskRunning;
extern std::mutex rgbTaskMutex;

typedef enum
{
    IwNone,
    IwRed,
    IwGreen,
    IwBlue,
    IwYellow,
    IwPink,
    IwCyan,
    IwWhite,
} IWColors; // IW as inkwatchy

void setRgb(IWColors color, bool clearPrevious = true, uint timeMs = 0);

#endif