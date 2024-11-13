#include "wFunctions.h"

void showFullWatchface()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        return;
    }
    return wManageOneDrawAll(wFO);
}

bool isModuleEngaged()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        return false;
    }
    return wFO->isModuleEngaged();
}

squareInfo getWatchModuleSquare()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        return {0, 0, 0, 0};
    }
    return wFO->watchfaceModSquare;
}

void cleanSomeDrawing()
{
    squareInfo square = getSomeDrawingSquare();
    display.fillRect(square.cord.x, square.cord.x, square.size.w, square.size.h, GxEPD_WHITE);
    // display.drawFastHLine(111, 61, 13, GxEPD_WHITE);
}

squareInfo getSomeDrawingSquare()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        return {0, 0, 0, 0};
    }
    return wFO->someDrawingSquare;
}