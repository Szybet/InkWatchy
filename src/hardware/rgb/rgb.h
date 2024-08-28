#ifndef RGB_H
#define RGB_H

#include "defines/defines.h"

typedef enum
{
    IwNone,
    IwRed,
    IwGreen,
    IwBlue,
} IWColors; // IW as inkwatchy

void setRgb(IWColors color);

#endif