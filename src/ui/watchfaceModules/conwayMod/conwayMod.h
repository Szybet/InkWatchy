#pragma once

#include "defines.h"

#if CONWAY_MODULE_ENABLED

#define INIT_CONWAY_MOD_VAL 200

// Because of rules in conway.cpp
#define CONWAY_MODULE_WIDTH 176
#define CONWAY_MODULE_HEIGHT 32

#define CONWAY_MODULE_OFFSET_X 1
#define CONWAY_MODULE_OFFSET_Y 4

void wfConwayrequestShow(buttonState button, bool *showBool);
void wfConwaycheckShow(bool *showBool, bool *redrawBool);

#endif

