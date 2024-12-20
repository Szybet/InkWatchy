#include "conwayMod.h"
#include "rtcMem.h"

#if CONWAY_MODULE_ENABLED

uint8_t conwayModuleNewGrid[CONWAY_MODULE_WIDTH / 8 * CONWAY_MODULE_HEIGHT]; // new grid doesn't need to be in rtc

void initModuleConway()
{
    debugLog("Initing conway module");
    initConwayGrid(rM.conwayModuleGrid, CONWAY_MODULE_GRID_PERCANTAGE, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH);
}

void moduleConwayGeneration()
{
    computeNewGeneration(rM.conwayModuleGrid, conwayModuleNewGrid, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH);
}

void wfConwaycheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
    if (rM.timeChangeCheck == INIT_CONWAY_MOD_VAL)
    {
#if CONWAY_CPU_SPEED
        getCpuSpeed();
        setCpuSpeed(maxSpeed);
#endif
        initModuleConway();
#if CONWAY_CPU_SPEED
        restoreCpuSpeed();
#endif
    }
    if (rM.timeChangeCheck != timeRTCLocal.Minute)
    {
        rM.timeChangeCheck = timeRTCLocal.Minute;
#if CONWAY_CPU_SPEED
        getCpuSpeed();
        setCpuSpeed(maxSpeed);
#endif
        moduleConwayGeneration();
#if CONWAY_CPU_SPEED
        restoreCpuSpeed();
#endif
        *redrawBool = true;
    }
#if CONWAY_MODULE_DEBUG
    *redrawBool = true;
#endif
}

void wfConwayrequestShow(buttonState button, bool *showBool)
{
    debugLog("Drawing conway module");
#if CONWAY_CPU_SPEED
    getCpuSpeed();
    setCpuSpeed(maxSpeed);
#endif
    if (button != None)
    {
        clearModuleArea();
        moduleConwayGeneration();
    }
    if (button == Menu)
    {
        initModuleConway();
    }

    squareInfo modSq = getWatchModuleSquare();
    drawGrid(rM.conwayModuleGrid, CONWAY_MODULE_HEIGHT, CONWAY_MODULE_WIDTH, modSq.cord.x + CONWAY_MODULE_OFFSET_X, modSq.cord.y + CONWAY_MODULE_OFFSET_Y);
    dUChange = true;
#if CONWAY_CPU_SPEED
    restoreCpuSpeed();
#endif
    debugLog("Finished drawing conway module");
}

#endif