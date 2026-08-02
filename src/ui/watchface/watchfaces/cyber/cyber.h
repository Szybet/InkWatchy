#pragma once

#include "defines.h"

#if WATCHFACE_CYBER

// This is a forward declaration
struct watchfaceDefOne;

const extern watchfaceDefOne cyberDefOne;

#define WATCHFACE_POSITIONS 3
#define EMPTY_POS 0
#define MODULE_POS 1
#define MODULE_ENG_POS 2

void cyberManageInput(buttonState bt);
void drawPosMarkerCyber();

#endif
