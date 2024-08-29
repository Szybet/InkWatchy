#ifndef RGB_H
#define RGB_H

#include "defines/defines.h"

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

void setRgb(IWColors color, bool clearPrevious = true);

#endif