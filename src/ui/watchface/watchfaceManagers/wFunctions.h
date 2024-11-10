#ifndef WFUNCTIONS_H
#define WFUNCTIONS_H

#include "../../../defines/defines.h"

void showFullWatchface(); // Can be also used by modules

// Module functions -- used by modules
bool isModuleEngaged();

// Extended modules size and place
#define SOME_RECT_X 0
#define SOME_RECT_Y 62
#define SOME_RECT_W 200
#define SOME_RECT_H 138
void cleanSomeDrawing(); // disableSomeDrawing controls

#endif
