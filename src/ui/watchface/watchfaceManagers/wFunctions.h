#pragma once

#include "defines.h"

void showFullWatchface(); // Can be also used by modules
void showTimeFullGlobal();

// Module functions -- used by modules
bool isModuleEngaged();
squareInfo getWatchModuleSquare();

// Extended modules size and place
void cleanSomeDrawing(); // disableSomeDrawing controls
squareInfo getSomeDrawingSquare();

