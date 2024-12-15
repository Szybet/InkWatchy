#pragma once

#include "inkField.h"

#if WATCHFACE_INKFIELD_SZYBET

#define WATCHFACE_POSITIONS 3 // There is one "empty" on 0 too ????????
#define EMPTY_POS 0
#define MODULE_POS 1
#define MODULE_ENG_POS 2

extern int watchfacePos;
extern bool positionEngaged;

void drawPosMarker();
void movePos(int add);
void inkFieldManageInput(buttonState bt);

#endif
