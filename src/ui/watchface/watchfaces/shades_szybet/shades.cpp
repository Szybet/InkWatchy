#include "shades.h"

#if WATCHFACE_SHADES_SZYBET

void showTimeFull()
{
    setTextSize(1);
    setFont(getFont("shades/RampartOne30"));
    display.fillRect(0, 0, 200, 55, GxEPD_WHITE);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), 16, 53);
}

#define SHADE_NORMAL_FONT "shades/RampartOne15"
#define SHADE_HEIGHT_PIXELS 30

RTC_DATA_ATTR uint16_t stepsSaved = 0;

const watchfaceDefOne shadesDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw)
    {
        uint16_t steps = getSteps();
        if(steps != stepsSaved || forceDraw == true) {
            stepsSaved = steps;
            setTextSize(1);
            setFont(getFont(SHADE_NORMAL_FONT));
            int y = 125;
            display.fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS, GxEPD_WHITE);
            writeTextCenterReplaceBack(String(steps) + " steps", y);
        } },
    .drawDay = []()
    {
        String dayDate = String(wFTime.Day);
        String month = getMonthName(wFTime.Month);
        String finalStr = dayDate + " " + month;
        setTextSize(1);
        setFont(getFont(SHADE_NORMAL_FONT));
        int y = 90;
        display.fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS, GxEPD_WHITE);
        writeTextCenterReplaceBack(finalStr, 85); },
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = []() {},
    .drawBattery = []()
    {
        setTextSize(1);
        setFont(getFont(SHADE_NORMAL_FONT));
        int y = 170;
        display.fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS, GxEPD_WHITE);
        writeTextCenterReplaceBack(String(batteryPercantageWF) + "%", y); },
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