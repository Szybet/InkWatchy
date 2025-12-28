#include "terrain.h"
#include "rtcMem.h"

#if WATCHFACE_TERRAIN

#define TIME_CORD_X 7
#define TIME_CORD_Y 118
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y
#define TIME_WIDTH 142
#define TIME_HEIGHT 33

// #define TIME_FONT getFont("terrain/font_terrain20")
#define TIME_FONT getFont("terrain/octosquares21")

void clearTime()
{
    dis->fillRect(TIME_CORD_X, TIME_CORD_Y - TIME_HEIGHT, TIME_WIDTH, TIME_HEIGHT, SCWhite);
}

void showTimeFull()
{
    setTextSize(1);
    setFont(TIME_FONT);
    clearTime();
    writeTextReplaceBack(getHourMinute(timeRTCLocal), TIME_CORD);
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("terrain/watchface"));
}

void drawBatteryThing(int x, int y)
{
    dis->drawPixel(x, y, SCBlack);
    dis->drawPixel(x + 5, y, SCBlack);
    dis->drawLine(x + 1, y - 1, x + 1 + 3, y - 1, SCBlack);
}

#define BATTERY_START_SHORT_Y 152
#define BATTERY_START_SHORT_HEIGHT 81
#define BATTERY_START_NORMAL_Y 153
#define BATTERY_START_NORMAL_HEIGHT 83

static void drawBattery()
{
    // Clear
    dis->drawLine(190, 152, 190, 72, SCWhite);
    dis->fillRect(192, 71, 3, 83, SCWhite);
    dis->drawLine(195, 152, 195, 72, SCWhite);

    // Draw those funny lines
    drawBatteryThing(190, 91);
    drawBatteryThing(190, 113);
    drawBatteryThing(190, 135);

    // Fill it
    {
        int upY = map(rM.bat.percentage, 0, 100, BATTERY_START_SHORT_Y, BATTERY_START_SHORT_Y - BATTERY_START_SHORT_HEIGHT);
        // debugLog("upY: " + String(upY) + " percent: " + String(rM.bat.percentage));
        dis->drawLine(190, BATTERY_START_SHORT_Y, 190, upY, SCBlack);
    }
    for (int i = 192; i < 195; i++)
    {
        int upY = map(rM.bat.percentage, 0, 100, BATTERY_START_NORMAL_Y, BATTERY_START_NORMAL_Y - BATTERY_START_NORMAL_HEIGHT);
        dis->drawLine(i, BATTERY_START_NORMAL_Y, i, upY, SCBlack);
    }
    {
        int upY = map(rM.bat.percentage, 0, 100, BATTERY_START_SHORT_Y, BATTERY_START_SHORT_Y - BATTERY_START_SHORT_HEIGHT);
        dis->drawLine(195, BATTERY_START_SHORT_Y, 195, upY, SCBlack);
    }
}

const watchfaceDefOne terrainDefOne = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw) {},
    .drawDay = []() {},
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
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
    .lpCoreScreenPrepareCustom = clearTime,
};

#endif
