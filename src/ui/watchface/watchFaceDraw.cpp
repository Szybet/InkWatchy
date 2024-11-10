#include "watchFaceDraw.h"

RTC_DATA_ATTR int percentOfDay;
RTC_DATA_ATTR int batteryPercantageWF;

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

void showFullWatchface()
{
    debugLog("Showing full watchface");
    
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

void cleanSomeDrawing()
{
    display.fillRect(SOME_RECT_X, SOME_RECT_Y, SOME_RECT_W, SOME_RECT_H, GxEPD_WHITE);
    display.drawFastHLine(111, 61, 13, GxEPD_WHITE);
}