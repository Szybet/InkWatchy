#include "inkField.h"
#include "inkput.h"
#include "inkWeather.h"
#include "rtcMem.h"

#if WATCHFACE_INKFIELD_SZYBET

#define TIME_FONT getFont("inkfield/JackInput40")
#define DATE_FONT getFont("JackInput17")
#define DAY_NAME_FONT getFont("inkfield/Speculum13")
#define MONTH_NAME_FONT getFont("inkfield/Speculum9")

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 46, 109
#define MONTH_NUMBER_1_CORD 91, 99
#define MONTH_NUMBER_2_CORD 91, 108
#define GENERAL_BAR_SIZE 54, 10
#define MONTH_BAR_CORD 136, 64
#define DAY_BAR_CORD 136, 64 + 15
#define BATT_BAR_CORD 136, 64 + 15 + 15
#define STEPS_BAR_CORD 136, 64 + 15 + 15 + 15

/*
// Even with monospaced font, it differs a bit...
{
  String who = "1234567890:";
  uint16_t ww;
  for (int i = 0; i < who.length(); i++)
  {
    String hh = String(who[i]);
    getTextBounds(hh, NULL, NULL, &ww, NULL);
    debugLog(hh + " " + String(ww));
  }
}

: 1 29
: 2 32
: 3 32
: 4 32
: 5 32
: 6 31
: 7 30
: 8 31
: 9 31
: 0 31
: : 12
*/

#define TIME_LETTERS_SPACING 32 // It differs, see below - so not it's static
#define TIME_CORD_X 11
#define TIME_CORD_Y 53
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y

static void drawTimeBeforeApply()
{
    debugLog("Called");
    setTextSize(1);
    setFont(TIME_FONT);
    debugLog("Getting hour minute for rM.wFTime");
    String oldTime = getHourMinute(rM.wFTime);
    debugLog("Getting hour minute for timeRTCLocal");
    String newTime = getHourMinute(timeRTCLocal);

    for (int i = 0; i < 5; i++)
    {
        if (oldTime[i] != newTime[i])
        {
            String toWrite = String(newTime[i]);
            String oldWrite = String(oldTime[i]);
            String beforeString = oldTime.substring(0, i);
            String afterString = oldTime.substring(0, i + 1);
            // debugLog("beforeString: " + beforeString);
            // debugLog("afterString: " + afterString);

            // uint16_t wBefore;
            // getTextBounds(beforeString, NULL, NULL, &wBefore, NULL);
            // debugLog("wBefore: " + String(wBefore));

            uint16_t wAfter;
            getTextBounds(afterString, NULL, NULL, &wAfter, NULL);
            // debugLog("wAfter: " + String(wAfter));

            uint16_t wToWrite;
            uint16_t hToWrite;
            getTextBounds(oldWrite, NULL, NULL, &wToWrite, &hToWrite);
            // debugLog("wToWrite: " + String(wToWrite));
            // debugLog("hToWrite: " + String(hToWrite));

            uint16_t finalWidthStart = wAfter - wToWrite;
            // debugLog("finalWidthStart: " + String(finalWidthStart));

            // debugLog("Writing to screen: " + toWrite);
            dis->fillRect(TIME_CORD_X + finalWidthStart, TIME_CORD_Y - hToWrite, TIME_LETTERS_SPACING, hToWrite, GxEPD_WHITE); // Clear things up
            writeTextReplaceBack(toWrite, TIME_CORD_X + finalWidthStart, TIME_CORD_Y);
        }
    }
}

static void drawTimeAfterApply(bool forceDraw)
{
    if (rM.inkfield.dayBar != timeRTCLocal.Day || forceDraw == true)
    {
        rM.inkfield.dayBar = timeRTCLocal.Day;
        uint8_t percentOfMonth = uint8_t(((float)rM.inkfield.dayBar / (float)31) * 100.0);
        drawProgressBar(MONTH_BAR_CORD, GENERAL_BAR_SIZE, percentOfMonth);
        inkDrawMoon();
    }

    // Draw the percentage on the right
    int percentOfDayTmp = calculatePercentageOfDay(rM.wFTime.Hour, rM.wFTime.Minute);
    if (rM.inkfield.percentOfDay != percentOfDayTmp || forceDraw == true)
    {
        rM.inkfield.percentOfDay = percentOfDayTmp;
        drawProgressBar(DAY_BAR_CORD, GENERAL_BAR_SIZE, rM.inkfield.percentOfDay);
    }

    uint16_t percentStepsTmp = uint16_t(((float)getSteps() / (float)STEPS_GOAL) * 100.0);
    debugLog("percentStepsTmp: " + String(percentStepsTmp));
    if (rM.inkfield.percentSteps != percentStepsTmp || forceDraw == true)
    {
        rM.inkfield.percentSteps = percentStepsTmp;
        drawProgressBar(STEPS_BAR_CORD, GENERAL_BAR_SIZE, rM.inkfield.percentSteps);
    }

    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
    // debugLog("Weather force: " + String(forceDraw));
    if (abs(rM.inkfield.weatherMinutes - weatherMinutes) > 25 || forceDraw == true)
    {
        rM.inkfield.weatherMinutes = weatherMinutes;
        inkDrawWeather();
    }
}

static void showTimeFull()
{
#if LP_CORE
    screenTimeChanged = true;
#endif
    // Now UI
    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), TIME_CORD);
}

static void initWatchface()
{
    writeImageN(0, 0, getImg("inkfield/watchface"));
    drawPosMarker();
}

static void drawDay()
{
    setFont(DATE_FONT);
    String dayDate = String(rM.wFTime.Day);
    if (dayDate.length() < 2)
    {
        dayDate = "0" + dayDate;
    }
    writeTextReplaceBack(dayDate, DATE_CORD);

    setFont(DAY_NAME_FONT);
    String day = getDayName();
    day.toUpperCase();

    String previousDay = getDayName(-1);
    previousDay.toUpperCase();
    uint16_t wDay;
    uint16_t hDay;
    getTextBounds(previousDay, NULL, NULL, &wDay, &hDay);
    dis->fillRect(DAY_NAME_CORD - hDay, wDay + 1, hDay + 1, GxEPD_WHITE); // Clear things up
    writeTextReplaceBack(day, DAY_NAME_CORD);
}

static void drawMonth()
{
    setFont(MONTH_NAME_FONT);
    String month = getMonthName(rM.wFTime.Month);
    month.toUpperCase();
    writeTextReplaceBack(month, MONTH_NAME_CORD);

    String realMonthNumber = String(rM.wFTime.Month + 1);
    if (realMonthNumber.length() == 1)
    {
        realMonthNumber = "0" + realMonthNumber;
    }

    setFont(getFont("dogicapixel4"));
    String f = String(realMonthNumber.substring(0, 1));
    String e = String(realMonthNumber.substring(1, 2));
    debugLog("f e: \"" + f + "\" and \"" + e + "\" Where full is: " + realMonthNumber);
    writeTextReplaceBack(f, MONTH_NUMBER_1_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1, 1);
    writeTextReplaceBack(e, MONTH_NUMBER_2_CORD, GxEPD_BLACK, GxEPD_WHITE, true, 1, 1);
}

static void drawBattery()
{
    drawProgressBar(BATT_BAR_CORD, GENERAL_BAR_SIZE, rM.batteryPercantageWF);
}

const watchfaceDefOne inkFieldDef = {
    .drawTimeBeforeApply = drawTimeBeforeApply,
    .drawTimeAfterApply = drawTimeAfterApply,
    .drawDay = drawDay,
    .drawMonth = drawMonth,
    .showTimeFull = showTimeFull,
    .initWatchface = initWatchface,
    .drawBattery = drawBattery,
    .manageInput = inkFieldManageInput,

    .watchfaceModules = true,
    .watchfaceModSquare = {.size{.w = 177, .h = 37}, .cord{.x = 7, .y = 160}},
    .someDrawingSquare = {.size{.w = 200, .h = 139}, .cord{.x = 0, .y = 61}},
    .isModuleEngaged = []()
    {
        if (rM.inkfield.watchfacePos == MODULE_ENG_POS && rM.inkfield.positionEngaged == true)
        {
            return true;
        }
        return false;
    }};

#endif