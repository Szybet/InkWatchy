#include "conwayMod.h"

#if CONWAY_MODULE_ENABLED

RTC_DATA_ATTR uint8_t timeChangeCheck = 0; // if 0, init the grid

// Because of rules in conway.cpp
#define CONWAY_MODULE_WIDTH 176
#define CONWAY_MODULE_HEIGHT 32

#define CONWAY_MODULE_OFFSET_X 1
#define CONWAY_MODULE_OFFSET_Y 4

RTC_DATA_ATTR uint8_t conwayModuleGrid[CONWAY_MODULE_WIDTH / 8 * CONWAY_MODULE_HEIGHT];
uint8_t conwayModuleNewGrid[CONWAY_MODULE_WIDTH / 8 * CONWAY_MODULE_HEIGHT]; // new grid doesn't need RTC_DATA_ATTR

void initModuleConway()
{
    debugLog("Initing conway module");
    initConwayGrid(conwayModuleGrid, CONWAY_MODULE_GRID_PERCANTAGE, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH);
}

void wfConwaycheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
    if (timeChangeCheck == 0)
    {
        initModuleConway();
    }
    if (timeChangeCheck != timeRTC->Minute)
    {
        timeChangeCheck = timeRTC->Minute;
        *redrawBool = true;
    }
}

void wfConwayrequestShow(buttonState button, bool *showBool)
{
    if (button != None)
    {
        clearModuleArea();
    }
    if (button == Menu)
    {
        initModuleConway();
    }
    computeNewGeneration(conwayModuleGrid, conwayModuleNewGrid, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH);
    drawGrid(conwayModuleGrid, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH, MODULE_RECT_X + CONWAY_MODULE_OFFSET_X, MODULE_RECT_Y + CONWAY_MODULE_OFFSET_Y);
    disUp(true);
}

RTC_DATA_ATTR wfModule wfConway = {
    true,
    wfConwaycheckShow,
    wfConwayrequestShow,
};

#endif