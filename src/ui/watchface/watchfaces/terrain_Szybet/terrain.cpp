#include "terrain.h"
#include "rtcMem.h"

#if WATCHFACE_TERRAIN

#define TIME_CORD_X 7
#define TIME_CORD_Y 118
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y
#define TIME_WIDTH 145
#define TIME_HEIGHT 30

// #define TIME_FONT getFont("terrain/font_terrain20")
#define TIME_FONT getFont("terrain/octosquares21")

void showTimeFull()
{
    setTextSize(1);
    setFont(TIME_FONT);
    // First clear because font sizes
    dis->fillRect(TIME_CORD_X, TIME_CORD_Y - TIME_HEIGHT, TIME_WIDTH, TIME_HEIGHT, SCWhite);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), TIME_CORD);
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("terrain/watchface"));
}

const watchfaceDefOne terrainDefOne = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw) {},
    .drawDay = []() {},
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = []() {},
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
    { return false; }};

#endif
