#include "shades.h"

#if WATCHFACE_SHADES_SZYBET

void showTimeFull()
{
    setTextSize(1);
    setFont(getFont("shades/RampartOne30"));
    display.fillRect(0, 0, 200, 55, GxEPD_WHITE);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), 16, 53);
}

const watchfaceDefOne shadesDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw) {},
    .drawDay = []() {},
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = []() {},
    .drawBattery = []() {},
    .manageInput = [](buttonState bt) {},

    .watchfaceModules = false,
    .watchfaceModSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .someDrawingSquare = {.size{.w = 0, .h = 0}, .cord{.x = 0, .y = 0}},
    .isModuleEngaged = []()
    { return false; }};

#endif