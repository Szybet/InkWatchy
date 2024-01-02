#include "watchface.h"

#define TIME_CORD 10, 53
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

void initWatchfaceDisplay()
{
  debugLog("Executing init watch face");
  wFTime = timeRTC;
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

  disUp(true);
}

void loopWatchfaceLoop()
{
  debugLog("Executing loop watch face");
  if (wFTime.Minute != timeRTC.Minute)
  {
    dUChange = true;
    wFTime.Minute = timeRTC.Minute;
    wFTime.Hour = timeRTC.Hour;

    setTextSize(1);
    setFont(TIME_FONT);
    writeTextReplaceBack(getHourMinute(), TIME_CORD);

    if (wFTime.Day != timeRTC.Day)
    {
      wFTime.Day = timeRTC.Day;
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
      writeTextReplaceBack(day, DAY_NAME_CORD);
    }

    if (wFTime.Month != timeRTC.Month)
    {
      wFTime.Month = timeRTC.Month;
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
  }

  if (batteryPercantageWF != bat.percentage)
  {
    batteryPercantageWF = bat.percentage;
    drawProgressBar(BATT_BAR_CORD, TO_DAY_BAR_SIZE, batteryPercantageWF);
    dUChange = true;
  }

  switch (useButton())
  {
  case Up:
  {
    break;
  }
  case Down:
  {
    break;
  }
  case Menu:
  {
    generalSwitch(mainMenu);
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
  }
  
  //if(dUChange == false) {
  //debugLog("No change in watchface, skipping timer");
  // Always go to sleep in watchface after loop
  sleepDelayMs = long(millis()) - SLEEP_EVERY_MS;
  //}

  disUp(dUChange, true);
}

int calculatePercentageOfDay(int hour, int minute)
{
  int totalMinutesInDay = 24 * 60;

  int currentTimeInMinutes = hour * 60 + minute;

  int percentageOfDayPassed = (currentTimeInMinutes * 100.0) / totalMinutesInDay;

  debugLog("Today day is in %: " + String(percentageOfDayPassed));

  return percentageOfDayPassed;
}
