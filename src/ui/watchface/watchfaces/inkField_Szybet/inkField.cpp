#include "inkField.h"
#include "inkput.h"
#include "inkWeather.h"
#include "inkField_localization.h"
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

#define TEMPS_TEXT_CORD 136, 117
#define TEMPS_BAR_CORD_X 136 + 21
#define TEMPS_BAR_CORD_Y 64 + 15 + 15 + 15
#define TEMPS_BAR_SIZE 33, 10

#define TIME_LETTERS_SPACING 36 // It differs, see above - so not it's static
#define TIME_CORD_X 11
#define TIME_CORD_Y 53
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y

// Helper function to get time string based on WATCHFACE_12H setting
String getInkFieldTimeString(tmElements_t timeEl) {
#if WATCHFACE_12H
    // 12-hour format without AM/PM
    int hour = timeEl.Hour;
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;
    
    String hourStr = String(hour);
    if (hourStr.length() == 1) hourStr = "0" + hourStr;  // Fixed: use "0" instead of " "
    
    String minuteStr = String(timeEl.Minute);
    if (minuteStr.length() == 1) minuteStr = "0" + minuteStr;
    
    return hourStr + ":" + minuteStr;
#else
    // 24-hour format
    String hourStr = String(timeEl.Hour);
    if (hourStr.length() == 1) hourStr = "0" + hourStr;
    
    String minuteStr = String(timeEl.Minute);
    if (minuteStr.length() == 1) minuteStr = "0" + minuteStr;
    
    return hourStr + ":" + minuteStr;
#endif
}

static void drawTimeBeforeApply()
{
    debugLog("Called");
    setTextSize(1);
    setFont(TIME_FONT);
    debugLog("Getting hour minute for rM.wFTime");
    String oldTime = getInkFieldTimeString(rM.wFTime);
    debugLog("Getting hour minute for timeRTCLocal");
    String newTime = getInkFieldTimeString(timeRTCLocal);

    for (int i = 0; i < 5; i++)
    {
        if (oldTime[i] != newTime[i])
        {
            String toWrite = String(newTime[i]);
            String oldWrite = String(oldTime[i]);
            String beforeString = oldTime.substring(0, i);
            String afterString = oldTime.substring(0, i + 1);

            uint16_t wAfter;
            getTextBounds(afterString, NULL, NULL, &wAfter, NULL);

            uint16_t wToWrite;
            uint16_t hToWrite;
            getTextBounds(oldWrite, NULL, NULL, &wToWrite, &hToWrite);

            uint16_t finalWidthStart = wAfter - wToWrite;

            dis->fillRect(TIME_CORD_X + finalWidthStart, TIME_CORD_Y - hToWrite, TIME_LETTERS_SPACING, hToWrite, SCWhite); // Clear things up
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

#if ACC_ENABLED
    uint16_t percentStepsTmp = uint16_t(((float)getSteps() / (float)STEPS_GOAL) * 100.0);
    debugLog("percentStepsTmp: " + String(percentStepsTmp));
    if (rM.inkfield.percentSteps != percentStepsTmp || forceDraw == true)
    {
        rM.inkfield.percentSteps = percentStepsTmp;
        drawProgressBar(STEPS_BAR_CORD, GENERAL_BAR_SIZE, rM.inkfield.percentSteps);
    }
#else
#if TEMP_CHECKS_ENABLED
    uint16_t tempsPercents = ((rM.previousTemp - TEMP_MINIMUM) * 100) / (TEMP_MAXIMUM - TEMP_MINIMUM);
    debugLog("tempsPercents: " + String(tempsPercents));
    if (rM.inkfield.showedTemp != tempsPercents || forceDraw == true)
    {
        rM.inkfield.showedTemp = tempsPercents;
        setTextSize(1);
        setFont(getFont("dogicapixel4"));
        writeTextReplaceBack("   ", TEMPS_TEXT_CORD);
        // Use formatTemperature for device temperature (not weather)
        String tempStr = formatTemperature(rM.previousTemp);
        if(tempStr.length() > 3) {
            tempStr = tempStr.substring(0, 3);
        }
        writeTextReplaceBack(tempStr, TEMPS_TEXT_CORD);
        drawProgressBar(TEMPS_BAR_CORD_X, TEMPS_BAR_CORD_Y, TEMPS_BAR_SIZE, rM.inkfield.showedTemp);
    }
#endif
#endif

    uint16_t weatherMinutes = timeRTCLocal.Minute + (60 * timeRTCLocal.Hour);
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
    // Show time in format based on WATCHFACE_12H setting (but no AM/PM)
    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getInkFieldTimeString(timeRTCLocal), TIME_CORD);
}

static void initWatchface()
{
#if ACC_ENABLED
    writeImageN(0, 0, getImg("inkfield/watchface"));
#else
    writeImageN(0, 0, getImg("inkfield/watchfaceNoSteps"));
#endif
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
    // Use localization.h function for day names
    String day = getLocalizedDayByIndex(timeRTCLocal.Wday, 0); // Current day
    day.toUpperCase();

    String previousDay = getLocalizedDayByIndex(timeRTCLocal.Wday, -1); // Previous day
    previousDay.toUpperCase();
    uint16_t wDay;
    uint16_t hDay;
    getTextBounds(previousDay, NULL, NULL, &wDay, &hDay);
    dis->fillRect(DAY_NAME_CORD - hDay, wDay + 1, hDay + 1, SCWhite); // Clear things up
    writeTextReplaceBack(day, DAY_NAME_CORD);
}

static void drawMonth()
{
    setFont(MONTH_NAME_FONT);
    // Use localized month names
    String month = getLocalizedMonthName(rM.wFTime.Month);
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
    writeTextReplaceBack(f, MONTH_NUMBER_1_CORD, SCBlack, SCWhite, true, 1, 1);
    writeTextReplaceBack(e, MONTH_NUMBER_2_CORD, SCBlack, SCWhite, true, 1, 1);
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
    },
    .lpCoreScreenPrepareCustom = NULL,
};

#endif
