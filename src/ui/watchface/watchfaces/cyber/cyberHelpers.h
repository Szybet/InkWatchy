#pragma once

#include "defines.h"

#if WATCHFACE_CYBER

int cyberCenterTextXOffset(String str);
int cyberRightTextXOffset(String str);
void cyberDrawArc(float start, float end, int x, int y, int radius);
void cyberDrawPctGraph(float pct, int x, int y);

#endif
