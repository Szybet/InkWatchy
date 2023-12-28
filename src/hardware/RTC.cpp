#include "RTC.h"

RTC_DATA_ATTR tmElements_t timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

void initRTC()
{
    SRTC.init();
    HWVer = SRTC.getWatchyHWVer();
    readRTC();
    SRTC.nextMinuteWake(true);
    pinMode(RTC_INT_PIN, INPUT);
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

void alarmManageRTC()
{
    if (digitalRead(RTC_INT_PIN) == LOW)
    {
        debugLog("RTC PIN IS HIGH");
        SRTC.clearAlarm();
        SRTC.nextMinuteWake(true);
        readRTC();
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

String getDayName(int date)
{
    switch (weekday(date))
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
        return "Inval";
    }
}

String getMonthName(int monthNumber) {
  switch (monthNumber) {
    case 1:
      return "Jan";
    case 2:
      return "Feb";
    case 3:
      return "Mar";
    case 4:
      return "Apr";
    case 5:
      return "May";
    case 6:
      return "Jun";
    case 7:
      return "Jul";
    case 8:
      return "Aug";
    case 9:
      return "Sep";
    case 10:
      return "Oct";
    case 11:
      return "Nov";
    case 12:
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
