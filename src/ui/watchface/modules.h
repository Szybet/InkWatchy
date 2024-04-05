#ifndef MODULES_LOGIC_H
#define MODULES_LOGIC_H

#include "../../defines/defines.h"

#define WATCHFACE_POSITIONS 2 // There is one "empty" on 0 too
#define EMPTY_POS 0
#define MODULE_POS 1

extern RTC_DATA_ATTR int watchfacePos;
extern RTC_DATA_ATTR bool positionEngaged;

void drawPosMarker();
void movePos(int add);
void modulesButtons(buttonState bt);

#endif
