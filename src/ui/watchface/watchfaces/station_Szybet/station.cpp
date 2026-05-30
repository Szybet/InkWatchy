#include "station.h"

#if WATCHFACE_STATION
#include "rtcMem.h"

#define STATION_NORMAL_FONT "station/Mono35"

static void showTimeFull()
{
    setTextSize(1);
    setFont(getFont(STATION_NORMAL_FONT));
    dis->fillRect(0, 0, 200, 180, SCWhite);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), 0, 117);
}

const watchfaceDefOne stationDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw) {},
    .drawDay = []() {},
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = []() {},
    .drawBattery = []()
    {
        dis->fillRect(0, 180, 200, 20, SCWhite);
        uint8_t batWidth = rM.bat.percentage * 2;
        dis->fillRect(0, 180, batWidth, 20, SCBlack); 
    },
    .manageInput = [](buttonState bt)
    {
    switch (bt)
    {
    case Menu:
    {
        generalSwitch(mainMenu);
        break;
    }
    default: {
        break;
    }
    }; },

    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = []()
    { return false; },
    .lpCoreScreenPrepareCustom = NULL,
};

#endif
