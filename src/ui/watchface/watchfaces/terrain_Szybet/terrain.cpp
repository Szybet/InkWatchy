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
#define STEPS_FONT getFont("terrain/octosquares11")
#define DAY_FONT getFont("terrain/fullmoon10")
#define DATE_FONT getFont("terrain/fullmoon9")

#define STEPS_CORD_X 1
#define STEPS_CORD_Y 25
#define STEPS_WIDTH 93
#define STEPS_HEIGHT 18

#define DAY_CORD_X 3
#define DAY_CORD_Y 144

#define DATE_CORD_X 3
#define DATE_CORD_Y 157

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

static void drawTimeAfterApply(bool forceDraw)
{
    // uint16_t percentStepsTmp = uint16_t(((float)getSteps() / (float)STEPS_GOAL) * 100.0);
    uint16_t steps = getSteps();
    if (rM.terrain.steps != steps || forceDraw == true)
    {
        rM.terrain.steps = steps;
        setTextSize(1);
        setFont(STEPS_FONT);
        // Clear the area
        dis->fillRect(STEPS_CORD_X, STEPS_CORD_Y - STEPS_HEIGHT, STEPS_WIDTH, STEPS_HEIGHT, SCWhite);
        String stepsStr = String(steps);
        while (stepsStr.length() < 5)
        {
            stepsStr = "0" + stepsStr;
        }
        writeTextReplaceBack(stepsStr, STEPS_CORD_X, STEPS_CORD_Y);

        // Bar
        writeImageN(4, 30, getImg("terrain/stepsbar"));
        uint16_t percentStepsTmp = uint16_t(((float)steps / (float)STEPS_GOAL) * 100.0);
        int toX = map(percentStepsTmp, 4, 103, 0, 100);
        // debugLog("toX: " + String(toX) + " percentStepsTmp: " + String(percentStepsTmp));
        dis->fillRect(4, 30, toX - 4, 5, SCBlack);
    }
}

String getDayByIndexTerrain(int dayOfWeek, int offset = 0)
{
    static const String dayNames[] = {
        "MONDAY",
        "TUESDAY",
        "WEDNESDAY",
        "THURSDAY",
        "FRIDAY",
        "SATURDAY",
        "SUNDAY"};

    // Apply offset and wrap around
    int dayIndex = (dayOfWeek + offset + 6) % 7;

    if (dayIndex >= 0 && dayIndex < 7)
    {
        return dayNames[dayIndex];
    }
    return "???";
}

static void drawDay()
{
    // Month
    setTextSize(1);
    setFont(DAY_FONT);
    dis->fillRect(DAY_CORD_X, DAY_CORD_Y, 90, 14, SCWhite);
    writeTextReplaceBack(getDayByIndexTerrain(timeRTCLocal.Wday), DAY_CORD_X, DAY_CORD_Y);

    // Date
    setFont(DATE_FONT);
    String month = getLocalizedMonthName(rM.wFTime.Month);
    month.toUpperCase();
    String dayDate = String(rM.wFTime.Day);
    if (dayDate.length() < 2)
    {
        dayDate = "0" + dayDate;
    }
    String year = "20" + String((timeRTCLocal.Year + 70) % 100);
    String finalDate = month + "-" + dayDate + "-" + year;
    writeTextReplaceBack(finalDate, DATE_CORD_X, DATE_CORD_Y);
}

const watchfaceDefOne terrainDefOne = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
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
