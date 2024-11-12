#ifndef WFUNCTIONS_H
#define WFUNCTIONS_H

#include "../../../defines/defines.h"

void showFullWatchface(); // Can be also used by modules

// Module functions -- used by modules
bool isModuleEngaged();
cordInfo getWatchModulePos();

// Extended modules size and place
void cleanSomeDrawing(); // disableSomeDrawing controls
squareInfo getSomeDrawingSize();

#endif
