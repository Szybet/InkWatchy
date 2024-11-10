#ifndef MODULES_LOGIC_H
#define MODULES_LOGIC_H

#include "inkField.h"

#define WATCHFACE_POSITIONS 3 // There is one "empty" on 0 too ????????
#define EMPTY_POS 0
#define MODULE_POS 1
#define MODULE_ENG_POS 2

extern int watchfacePos;
extern bool positionEngaged;

void drawPosMarker();
void movePos(int add);
void modulesButtons(buttonState bt);

#endif
