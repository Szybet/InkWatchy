#include "inkField.h"
#include "inkput.h"

RTC_DATA_ATTR int percentOfDay;

#define TIME_FONT getFont("JackInput40")
#define DATE_FONT getFont("JackInput17")
#define DAY_NAME_FONT getFont("Speculum13")
#define MONTH_NAME_FONT getFont("Speculum9")

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 46, 109
#define MONTH_NUMBER_1_CORD 89, 93 // Not used?
#define MONTH_NUMBER_2_CORD 89, 102 // Not used?
#define TO_DAY_BAR_CORD 136, 68
#define TO_DAY_BAR_SIZE 54, 6
#define BATT_BAR_CORD 136, 83

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

void drawTimeBeforeApply()
{
    debugLog("Called");
    setTextSize(1);
    setFont(TIME_FONT);
    debugLog("Getting hour minute for wFTime");
    String oldTime = getHourMinute(wFTime);
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
            display.fillRect(TIME_CORD_X + finalWidthStart, TIME_CORD_Y - hToWrite, TIME_LETTERS_SPACING, hToWrite, GxEPD_WHITE); // Clear things up
            writeTextReplaceBack(toWrite, TIME_CORD_X + finalWidthStart, TIME_CORD_Y);
        }
    }
}

void drawTimeAfterApply()
{
    // Draw the percentage on the right
    int percentOfDayTmp = calculatePercentageOfDay(wFTime.Hour, wFTime.Minute);
    if (percentOfDay != percentOfDayTmp)
    {
        percentOfDay = percentOfDayTmp;
        drawProgressBar(TO_DAY_BAR_CORD, TO_DAY_BAR_SIZE, percentOfDay);
    }
}

void showTimeFull()
{
#if LP_CORE
    screenTimeChanged = true;
#endif
    // Now UI
    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(timeRTCLocal), TIME_CORD);
}

void inkShowFullWatchface()
{
    writeImageN(0, 0, getImg("watchface"));

    showTimeFull();

    setTextSize(1);
    setFont(DATE_FONT);
    String dayDate = String(wFTime.Day);
    if (dayDate.length() < 2)
    {
        dayDate = "0" + dayDate;
    }
    writeTextReplaceBack(dayDate, DATE_CORD);

    setTextSize(1);
    setFont(DAY_NAME_FONT);
    String day = getDayName();
    day.toUpperCase();
    writeTextReplaceBack(day, DAY_NAME_CORD);

    setTextSize(1);
    setFont(MONTH_NAME_FONT);
    String month = getMonthName(wFTime.Month);
    month.toUpperCase();
    writeTextReplaceBack(month, MONTH_NAME_CORD);

    percentOfDay = calculatePercentageOfDay(wFTime.Hour, wFTime.Minute);
    drawProgressBar(TO_DAY_BAR_CORD, TO_DAY_BAR_SIZE, percentOfDay);

    batteryPercantageWF = bat.percentage;
    drawProgressBar(BATT_BAR_CORD, TO_DAY_BAR_SIZE, batteryPercantageWF);

    wfModulesManage(None, true);
    drawModuleCount(true);
    drawPosMarker();
}

void drawDay()
{
    setFont(DATE_FONT);
    String dayDate = String(wFTime.Day);
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
    display.fillRect(DAY_NAME_CORD - hDay, wDay + 1, hDay + 1, GxEPD_WHITE); // Clear things up
    writeTextReplaceBack(day, DAY_NAME_CORD);
}

void drawMonth()
{
    setFont(MONTH_NAME_FONT);
    String month = getMonthName(wFTime.Month);
    month.toUpperCase();
    writeTextReplaceBack(month, MONTH_NAME_CORD);
}

void drawBattery()
{
    drawProgressBar(BATT_BAR_CORD, TO_DAY_BAR_SIZE, batteryPercantageWF);
}

/*
    void (*drawTimeBeforeApply)(); // Draw the time. Here you can compare the times to draw only whats needed, or just draw it all
    void (*drawTimeAfterApply)(); // Do things after time is applied. Like update the step counter
    void (*drawDay)(); //  Do things after the day changed, like update the date
    void (*drawMonth)(); // Do things after the month changed, like updating the month
    void (*showTimeFull)(); // Fully draw the time. Used bto handle  the lp core on yatchy, you can call this function on drawTimeAfterApply simply
    void (*initWatchface)(); // Called on init once. Draw here static images and all other things
    void (*drawBattery)(); // Draw the battery, it's to avoid logic replication as every watchface will show it. This function is only called when battery percentage changed
    void (*manageInput)(buttonState bt);

    bool watchfaceModules; // To enable modules. All things below are only used if this is enabled
    cordInfo watchfacePos; // Position of watchface modules
    void (*cleanSomeDrawing)(); // TODO: look into it to properly support watchface modules

const watchfaceDefOne inkFieldDef = {
    .drawTimeBeforeApply = 
}
*/

const watchfaceDefOne inkFieldDef = {
    .drawTimeBeforeApply = []() { 
        /* Implementation for drawing time before apply */
        debugLog("Nice. Nice. Nice.");
    },
    .drawTimeAfterApply = []() { /* Implementation for drawing time after apply */ },
    .drawDay = []() { /* Implementation for drawing day */ },
    .drawMonth = []() { /* Implementation for drawing month */ },
    .showTimeFull = []() { /* Implementation for fully drawing time */ },
    .initWatchface = []() { /* Implementation for initializing watchface */ },
    .drawBattery = []() { /* Implementation for drawing battery */ },
    .manageInput = [](buttonState bt) { /* Implementation for managing input */ },

    .watchfaceModules = true, // Set watchfaceModules to true or false as needed
    .watchfacePos = {0, 0}, // Initialize watchfacePos with appropriate values
    .cleanSomeDrawing = []() { /* Implementation for cleaning some drawing */ },
    .isModuleEngaged = []() { return false; /* Implementation for checking if module is engaged */ }
};