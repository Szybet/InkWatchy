#include "wFunctions.h"

void showFullWatchface() {
    
}

bool isModuleEngaged() {
    return false;
}

void cleanSomeDrawing()
{
    display.fillRect(SOME_RECT_X, SOME_RECT_Y, SOME_RECT_W, SOME_RECT_H, GxEPD_WHITE);
    display.drawFastHLine(111, 61, 13, GxEPD_WHITE);
}