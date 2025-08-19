#include "shades.h"
#include "rtcMem.h"

#if WATCHFACE_SHADES_SZYBET

void showTimeFull()
{
    setTextSize(1);
    setFont(getFont("shades/RampartOne30"));
    dis->fillRect(0, 0, 200, 55, SCWhite);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), 16, 53);
}

#define SHADE_NORMAL_FONT "shades/RampartOne15"
#define SHADE_HEIGHT_PIXELS 30
#define BOTTOM_ADD 5

const watchfaceDefOne shadesDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = [](bool forceDraw)
    {
        uint16_t steps = getSteps();
        if(steps != rM.shades.stepsSaved || forceDraw == true) {
            rM.shades.stepsSaved = steps;
            setTextSize(1);
            setFont(getFont(SHADE_NORMAL_FONT));
            int y = 125;
            dis->fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS + BOTTOM_ADD, SCWhite);
            writeTextCenterReplaceBack(String(steps) + " steps", y);
        } },
    .drawDay = []()
    {
        String dayDate = String(rM.wFTime.Day);
        String month = getMonthName(rM.wFTime.Month);
        String finalStr = dayDate + " " + month;
        setTextSize(1);
        setFont(getFont(SHADE_NORMAL_FONT));
        int y = 90;
        dis->fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS + BOTTOM_ADD, SCWhite);
        writeTextCenterReplaceBack(finalStr, 85); },
    .drawMonth = []() {},
    .showTimeFull = showTimeFull,
    .initWatchface = []() {},
    .drawBattery = []()
    {
        setTextSize(1);
        setFont(getFont(SHADE_NORMAL_FONT));
        int y = 170;
        dis->fillRect(0, y - SHADE_HEIGHT_PIXELS, 200, SHADE_HEIGHT_PIXELS + BOTTOM_ADD, SCWhite);
        writeTextCenterReplaceBack(String(rM.batteryPercantageWF) + "%", y); },
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