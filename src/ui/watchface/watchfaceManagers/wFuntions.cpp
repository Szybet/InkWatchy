#include "wFunctions.h"

void showFullWatchface() {
    const watchfaceDefOne * wFO = getwatchfaceDefOne();
    if(wFO == NULL) {
        return;
    }
    return wManageOneDrawAll(wFO);
}

bool isModuleEngaged() {
    const watchfaceDefOne * wFO = getwatchfaceDefOne();
    if(wFO == NULL) {
        return false;
    }
    return wFO->isModuleEngaged();
}

cordInfo getWatchModulePos() {
    const watchfaceDefOne * wFO = getwatchfaceDefOne();
    if(wFO == NULL) {
        return {0,0};
    }
    return wFO->watchfaceModPos;
}

void cleanSomeDrawing()
{
    squareInfo square = getSomeDrawingSize();
    display.fillRect(square.cord.x, square.cord.x, square.size.w, square.size.h, GxEPD_WHITE);
    // display.drawFastHLine(111, 61, 13, GxEPD_WHITE);
}

squareInfo getSomeDrawingSize() {
    const watchfaceDefOne * wFO = getwatchfaceDefOne();
    if(wFO == NULL) {
        return {0,0,0,0};
    }
    return wFO->someDrawingSize;
}