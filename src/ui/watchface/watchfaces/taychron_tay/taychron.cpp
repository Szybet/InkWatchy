#include "taychron.h"
#include "tayWeather.h"
#include "taychron_localization.h"
#include <MoonPhase.h>
#include "rtcMem.h"

#if WATCHFACE_TAYCHRON

#define TIME_FONT getFont("taychron/Mono30")

#define GENERAL_BAR_SIZE 97, 16
#define MONTH_BAR_CORD 60, 5
#define BATT_BAR_CORD 60, 5 + 20

#define TIME_CORD_X 18
#if WATCHFACE_TAYCHRON_DATE
// with date
#define TIME_CORD_Y 110
#define DATE_CORD_Y (TIME_CORD_Y + 25)
#define DATE_CORD_X 75
#define DATE_FONT getFont("taychron/Mono13")
#else
// without date
#define TIME_CORD_Y 119
#endif

#if WATCHFACE_12H
#define AMPPM_X 150
#define AMPPM_Y 143
#define AMPM_FONT getFont("taychron/Mono13")
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
    dis->fillRect(0, TIME_CORD_Y, 200, 30, GxEPD_BLACK);            // Clear middle
    String currTime = getTaychronLocalizedTimeString(timeRTCLocal); // Use localized function

#if WATCHFACE_12H
    setTextSize(1);
    setFont(AMPM_FONT);
    String ampmStr = getTaychronLocalizedAMPM(timeRTCLocal); // Use localized function
    writeTextReplaceBack(ampmStr, AMPPM_X, AMPPM_Y, GxEPD_WHITE, GxEPD_BLACK);
#endif

    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(currTime, TIME_CORD_X, TIME_CORD_Y, GxEPD_WHITE, GxEPD_BLACK);

#if WATCHFACE_TAYCHRON_DATE
    // lets try drawing date below it
    String day = String(rM.wFTime.Day);
    String month = getLocalizedMonthName(rM.wFTime.Month);
    String finalStr = day + ". " + month;

    setTextSize(1);
    setFont(DATE_FONT);
    writeTextReplaceBack(finalStr, DATE_CORD_X, DATE_CORD_Y, GxEPD_WHITE, GxEPD_BLACK);
#endif
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
