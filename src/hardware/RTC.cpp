#include "RTC.h"

RTC_DATA_ATTR tmElements_t timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
  pinMode(RTC_INT_PIN, INPUT);
  if (isFromWakeUp == false)
  {
    SRTC.init();
    HWVer = SRTC.getWatchyHWVer();
  }
  if (wakeUpReason != RTC_WAKEUP_REASON)
  {
    readRTC();
    wakeUpManageRTC();
  }
}

void saveRTC()
{
  SRTC.set(timeRTC);
}

void readRTC()
{
  debugLog("Reading RTC");
  SRTC.read(timeRTC);
}

void wakeUpManageRTC()
{
  SRTC.clearAlarm();
  if (NIGHT_SLEEP_FOR_M != 1 && (timeRTC.Hour >= NIGHT_SLEEP_AFTER_HOUR || timeRTC.Hour < NIGHT_SLEEP_BEFORE_HOUR))
  {
    debugLog("Next wake up in " + String(NIGHT_SLEEP_FOR_M) + " minutes");
    SRTC.atMinuteWake(timeRTC.Minute + NIGHT_SLEEP_FOR_M, true);
  }
  else
  {
    debugLog("Next minute wake up");
    SRTC.nextMinuteWake(true);
  }
}

void alarmManageRTC()
{
  if (digitalRead(RTC_INT_PIN) == LOW)
  {
    debugLog("RTC PIN IS HIGH");
    readRTC();
    wakeUpManageRTC();
  }
}

String getHourMinute()
{
  String h = String(timeRTC.Hour);
  if (h.length() == 1)
  {
    h = "0" + h;
  }

  String m = String(timeRTC.Minute);
  if (m.length() == 1)
  {
    m = "0" + m;
  }

  String answer = h + ":" + m;
  debugLog("Answer: " + answer);
  return answer;
}

String getDayName()
{
  int unixTime = SRTC.MakeTime(timeRTC);
  int weekDay = weekday(unixTime);
  debugLog("unixTime: " + String(unixTime));
  debugLog("weekDay reported: " + String(weekDay));
  switch (weekDay)
  {
  case 1:
    return "Sun";
  case 2:
    return "Mon";
  case 3:
    return "Tues";
  case 4:
    return "Wedne";
  case 5:
    return "Thurs";
  case 6:
    return "Fri";
  case 7:
    return "Satur";
  default:
    return "How??";
  }
}

String getMonthName(int monthNumber)
{
  debugLog("Month gived: " + String(monthNumber));
  switch (monthNumber)
  {
  case 0:
    return "Jan";
  case 1:
    return "Feb";
  case 2:
    return "Mar";
  case 3:
    return "Apr";
  case 4:
    return "May";
  case 5:
    return "Jun";
  case 6:
    return "Jul";
  case 7:
    return "Aug";
  case 8:
    return "Sep";
  case 9:
    return "Oct";
  case 10:
    return "Nov";
  case 11:
    return "Dec";
  default:
    return "Inv";
  }
}

#if DEBUG
void initRTCDebug()
{
  debugLog("Get RTC battery level: " + String(SRTC.getRTCBattery(false)));
  debugLog("Get critical RTC battery level: " + String(SRTC.getRTCBattery(true)));
}

void loopRTCDebug()
{
  debugLog("SRTC.isOperating: " + BOOL_STR(SRTC.isOperating()));
  debugLog("SRTC.getADCPin: " + String(SRTC.getADCPin()));
  debugLog("SRTC.temperature(): " + String(SRTC.temperature()));
}

void dumpRTCTime()
{
  readRTC();
  debugLog("Second: " + String(timeRTC.Second));
  debugLog("Minute: " + String(timeRTC.Minute));
  debugLog("Hour: " + String(timeRTC.Hour));
  debugLog("Day: " + String(timeRTC.Day));
  debugLog("Month: " + String(timeRTC.Month));
  debugLog("Year: " + String(timeRTC.Year));
}
#endif
