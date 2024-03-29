#include "watchface.h"

#define TIME_LETTERS_SPACING 32 // It differs, see below - so not it's static
#define TIME_CORD_X 11
#define TIME_CORD_Y 53
#define TIME_CORD TIME_CORD_X, TIME_CORD_Y

#define DAY_NAME_CORD 13, 87
#define DATE_CORD 8, 113
#define MONTH_NAME_CORD 48, 109
#define MONTH_NUMBER_1_CORD 89, 93
#define MONTH_NUMBER_2_CORD 89, 102

#define TO_DAY_BAR_CORD 136, 68
#define TO_DAY_BAR_SIZE 54, 6

#define BATT_BAR_CORD 135, 83

#define TIME_FONT &JackInput40pt7b
#define DATE_FONT &JackInput17pt7b
#define DAY_NAME_FONT &Speculum13pt7b
#define MONTH_NAME_FONT &Speculum9pt7b

RTC_DATA_ATTR tmElements_t wFTime;
RTC_DATA_ATTR int percentOfDay;
RTC_DATA_ATTR int batteryPercantageWF;

#define WATCHFACE_POSITIONS 2 // There is one "empty" on 0 too
#define EMPTY_POS 0
#define MODULE_POS 1
RTC_DATA_ATTR int watchfacePos = 0;
RTC_DATA_ATTR bool positionEngaged = false;

void drawPosMarker()
{
  // First clean all Pos markers
  display.drawPixel(190, 160, GxEPD_WHITE);
  display.drawPixel(191, 160, GxEPD_WHITE);
  display.drawPixel(191, 161, GxEPD_WHITE);

  display.drawPixel(108, 60, GxEPD_WHITE);
  display.drawPixel(109, 60, GxEPD_WHITE);
  display.drawPixel(108, 59, GxEPD_WHITE);

  // Then draw the one
  if (watchfacePos == MODULE_POS)
  {
    // 190, 160
    // 191, 160
    // 191, 161
    display.drawPixel(190, 160, GxEPD_BLACK);
    display.drawPixel(191, 160, GxEPD_BLACK);
    display.drawPixel(191, 161, GxEPD_BLACK);
  } else if(watchfacePos == EMPTY_POS) {
    // 108, 60
    // 109, 60
    // 108, 59
    display.drawPixel(108, 60, GxEPD_BLACK);
    display.drawPixel(109, 60, GxEPD_BLACK);
    display.drawPixel(108, 59, GxEPD_BLACK);
  }
  dUChange = true;
}

void movePos(int add) {
  watchfacePos = watchfacePos + add;
  checkMaxMin(&watchfacePos, WATCHFACE_POSITIONS - 1);
  drawPosMarker();
}

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

void writeTimeMinimal()
{
  debugLog("Called");
  setTextSize(1);
  setFont(TIME_FONT);
  String oldTime = getHourMinute(&wFTime);
  String newTime = getHourMinute();

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

  wFTime.Minute = timeRTC->Minute;
  wFTime.Hour = timeRTC->Hour;
}

void initWatchfaceDisplay()
{
  debugLog("Executing init watch face");
  readRTC(); // For returning from book, for example
  // Idk?
  // wFTime = timeRTC;
  wFTime.Second = timeRTC->Second;
  wFTime.Minute = timeRTC->Minute;
  wFTime.Hour = timeRTC->Hour;
  wFTime.Day = timeRTC->Day;
  wFTime.Month = timeRTC->Month;
  wFTime.Year = timeRTC->Year;
  // dumpRTCTime(wFTime);
  // dumpRTCTime(timeRTC);

  writeImageN(0, 0, watchfaceImgPack);

  setTextSize(1);
  setFont(TIME_FONT);
  writeTextReplaceBack(getHourMinute(), TIME_CORD);

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

  drawPosMarker();

  disUp(true, false, true);
}

// Basically reasons for it to be not in normal mode (sleeping)
bool isMoreActive()
{
  return bat.isCharging;
}

bool wentToSleep = false; // Don't go to sleep after one try of noClickedButton - maybe a sync is going on?
void loopWatchfaceLoop()
{
  if (isMoreActive() == true)
  {            // Here should be a checker if its in these hours when wakeups are disabled, otherwise in main wakeup manage will read it, but this is fine too
    readRTC(); // It's really only needed when wifi is on and its charging
    wfModulesManage(None);
  }

  // debugLog("Executing loop watch face");
  if (wFTime.Minute != timeRTC->Minute)
  {
    dUChange = true;

    writeTimeMinimal();

    if (wFTime.Day != timeRTC->Day)
    {
      wFTime.Day = timeRTC->Day;
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

    if (wFTime.Month != timeRTC->Month)
    {
      wFTime.Month = timeRTC->Month;
      setFont(MONTH_NAME_FONT);
      String month = getMonthName(wFTime.Month);
      month.toUpperCase();
      writeTextReplaceBack(month, MONTH_NAME_CORD);
    }

    int percentOfDayTmp = calculatePercentageOfDay(wFTime.Hour, wFTime.Minute);
    if (percentOfDay != percentOfDayTmp)
    {
      percentOfDay = percentOfDayTmp;
      drawProgressBar(TO_DAY_BAR_CORD, TO_DAY_BAR_SIZE, percentOfDay);
    }

    wfModulesManage(None);
  }

  if (batteryPercantageWF != bat.percentage)
  {
    batteryPercantageWF = bat.percentage;
    drawProgressBar(BATT_BAR_CORD, TO_DAY_BAR_SIZE, batteryPercantageWF);
    dUChange = true;
  }

  bool buttonClicked = false;
  buttonState bt = useButton();
  if(bt == None) {
    buttonClicked = true;
  }
  switch (bt)
  {
  case Up:
  {
    if(positionEngaged == false) {
      movePos(1);
    } else {
      if(watchfacePos == MODULE_POS) {
        wfModuleSwitch(Right);
        drawModuleCount();
      }
    }
    break;
  }
  case Down:
  {
    if(positionEngaged == false) {
      movePos(-1);
    } else {
      if(watchfacePos == MODULE_POS) {
        wfModuleSwitch(Left);
        drawModuleCount();
      }
    }
    break;
  }
  case Menu:
  {
    if(watchfacePos == EMPTY_POS) {
      generalSwitch(mainMenu);
    } else if(watchfacePos == MODULE_POS) {
      wfModulesManage(Menu);
      drawModuleCount();
    }
    break;
  }
  case LongUp:
  {

    break;
  }
  case LongDown:
  {

    break;
  }
  case LongMenu:
  {
    positionEngaged = !positionEngaged;
    break;
  }
  }

  // if(dUChange == false) {
  // debugLog("No change in watchface, skipping timer");
  // Always go to sleep in watchface after loop
  // Well not if it's charging
  if (buttonClicked == false && wentToSleep == false && (bat.isCharging == false || SYNC_WIFI == 0))
  {
    sleepDelayMs = long(millis()) - SLEEP_EVERY_MS;
    wentToSleep = true;
  }
  //}

  disUp(dUChange, true, true);
}

int calculatePercentageOfDay(int hour, int minute)
{
  int totalMinutesInDay = 24 * 60;

  int currentTimeInMinutes = hour * 60 + minute;

  int percentageOfDayPassed = (currentTimeInMinutes * 100.0) / totalMinutesInDay;

  debugLog("Today day is in %: " + String(percentageOfDayPassed));

  return percentageOfDayPassed;
}
