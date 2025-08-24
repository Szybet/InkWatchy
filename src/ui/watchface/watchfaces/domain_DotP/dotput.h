#pragma once

#include "domain.h"

#if WATCHFACE_DOMAIN_DOTP

#define WATCHFACE_POSITIONS 3 // There is one "empty" on 0 too ????????
#define EMPTY_POS 0
#define MODULE_POS 1
#define MODULE_ENG_POS 2

void domainDrawPosMarker();
void domainMovePos(int add);
void domainManageInput(buttonState bt);

#endif