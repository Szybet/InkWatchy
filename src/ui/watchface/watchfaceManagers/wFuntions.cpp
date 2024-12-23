#include "wFunctions.h"

void showFullWatchface()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        debugLog("Not implemented");
        return;
    }
    return wManageOneDrawAll(wFO);
}

void showTimeFullGlobal() {
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        debugLog("Not implemented");
        return;
    }
    wFO->showTimeFull();
}

bool isModuleEngaged()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        debugLog("Not implemented");
        return false;
    }
    return wFO->isModuleEngaged();
}

squareInfo getWatchModuleSquare()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        debugLog("Not implemented");
        return {0, 0, 0, 0};
    }
    return wFO->watchfaceModSquare;
}

void cleanSomeDrawing()
{
    squareInfo square = getSomeDrawingSquare();
    dis->fillRect(square.cord.x, square.cord.y, square.size.w, square.size.h, GxEPD_WHITE);
}

squareInfo getSomeDrawingSquare()
{
    const watchfaceDefOne *wFO = getwatchfaceDefOne();
    if (wFO == NULL)
    {
        debugLog("Not implemented");
        return {0, 0, 0, 0};
    }
    return wFO->someDrawingSquare;
}