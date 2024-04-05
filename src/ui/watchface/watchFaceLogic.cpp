#include "watchface.h"

int calculatePercentageOfDay(int hour, int minute)
{
  int totalMinutesInDay = 24 * 60;

  int currentTimeInMinutes = hour * 60 + minute;

  int percentageOfDayPassed = (currentTimeInMinutes * 100.0) / totalMinutesInDay;

  debugLog("Today day is in %: " + String(percentageOfDayPassed));

  return percentageOfDayPassed;
}
