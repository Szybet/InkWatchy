#include "taychron.h"
#include "tayWeather.h"
#include <MoonPhase.h>
#include "rtcMem.h"

#if WATCHFACE_TAYCHRON

#define TIME_FONT getFont("taychron/Mono30")

#define GENERAL_BAR_SIZE 97, 16
#define MONTH_BAR_CORD 60, 5
#define BATT_BAR_CORD 60, 5 + 20

#define TIME_CORD_X 18
#define TIME_CORD_Y 119

#if WATCHFACE_12H
#define AMPPM_X 150
#define AMPPM_Y 143
#define AMPM_FONT getFont("taychron/Mono13")

String getAMPM(String time24)
{
    // Extract the hour from the 24-hour time string
    int colonIndex = time24.indexOf(':');
    int hour = time24.substring(0, colonIndex).toInt();

    // Determine AM or PM
    if (hour < 12)
    {
        return "AM";
    }
    else
    {
        return "PM";
    }
}

String convertTo12HourFormat(String time24)
{
    // Split the input time (hh:mm) into hours and minutes
    int colonIndex = time24.indexOf(':');
    int hour = time24.substring(0, colonIndex).toInt();
    String minute = time24.substring(colonIndex + 1);

    // Convert the hour to 12-hour format
    if (hour == 0)
    {
        hour = 12; // Midnight case
    }
    else if (hour > 12)
    {
        hour -= 12; // Afternoon case
    }

    // Add leading zero to the hour if necessary
    String hourStr = String(hour);
    if (hourStr.length() == 1)
    {
        hourStr = "0" + hourStr;
    }

    // Combine hour and minute for the final result
    return hourStr + ":" + minute;
}
#endif

static void drawTimeAfterApply(bool forceDraw)
{
    if (rM.taychron.dayBar != timeRTCLocal.Day || forceDraw == true)
    {
        rM.taychron.dayBar = timeRTCLocal.Day;
        uint8_t percentOfMonth = uint8_t(((float)rM.taychron.dayBar / (float)31) * 100.0);
        drawProgressBar(MONTH_BAR_CORD, GENERAL_BAR_SIZE, percentOfMonth);
        tayDrawMoon();
    }

    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
    // debugLog("Weather force: " + String(forceDraw));
    if (abs(rM.taychron.weatherMinutes - weatherMinutes) > 25 || forceDraw == true)
    {
        rM.taychron.weatherMinutes = weatherMinutes;
        tayDrawWeather();
    }
}

static void showTimeFull()
{
#if LP_CORE
    screenTimeChanged = true;
#endif
    // Now UI
    dis->fillRect(0, TIME_CORD_Y, 200, 30, GxEPD_BLACK); // Clear middle
    String currTime = getHourMinute(timeRTCLocal);

#if WATCHFACE_12H
    setTextSize(1);
    setFont(AMPM_FONT);
    writeTextReplaceBack(getAMPM(currTime), AMPPM_X, AMPPM_Y, GxEPD_WHITE, GxEPD_BLACK);

    currTime = convertTo12HourFormat(currTime);
#endif

    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(currTime, TIME_CORD_X, TIME_CORD_Y, GxEPD_WHITE, GxEPD_BLACK);
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("taychron/watchface"));
    readRTC();
}

static void drawBattery()
{
    drawProgressBar(BATT_BAR_CORD, GENERAL_BAR_SIZE, rM.batteryPercantageWF);
}

const watchfaceDefOne taychronDef = {
    .drawTimeBeforeApply = showTimeFull,
    .drawTimeAfterApply = drawTimeAfterApply,
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
#if LONG_BACK_FULL_REFRESH
    case LongBack:
    {
        debugLog("Because of LONG_BACK_FULL_REFRESH in watchface, I shall now refresh the screen fully");
        updateDisplay(FULL_UPDATE);
        break;
    }
#endif
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