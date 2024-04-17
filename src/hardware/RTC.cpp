#include "RTC.h"

tmElements_t* timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

void setupTimeStructure() {
  debugLog("Created time memory");
  timeRTC = new tmElements_t;
}

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
    setupTimeStructure();
    readRTC();
    wakeUpManageRTC();
  }
}

void saveRTC()
{
  SRTC.set(*timeRTC);

  // Test
  /*
  debugLog("Time test:");
  SRTC.read(*timeRTC);
  dumpRTCTimeSmall(timeRTC);
  delay(1000);
  SRTC.read(*timeRTC);
  dumpRTCTimeSmall(timeRTC);
  SRTC.read(*timeRTC);
  dumpRTCTimeSmall(timeRTC);
  */
}

void readRTC()
{
  //debugLog("Reading RTC");
  uint8_t m = timeRTC->Minute;
  SRTC.read(*timeRTC);
  if(m != timeRTC->Minute) {
    checkDrift();
  }
}

void wakeUpManageRTC()
{
  SRTC.clearAlarm();
  if (disableWakeUp == false)
  {
    //isDebug(dumpRTCTime());
    int hour = timeRTC->Hour;
    //debugLog("timeRTC->Hour: " + String(hour));
    // Watchy 2.0 has problems here... Idk?
    if (NIGHT_SLEEP_FOR_M != 1 && (hour >= NIGHT_SLEEP_AFTER_HOUR || hour < NIGHT_SLEEP_BEFORE_HOUR))
    {
      //debugLog("Next wake up in " + String(NIGHT_SLEEP_FOR_M) + " minutes");
      //isDebug(dumpRTCTime());
      SRTC.atMinuteWake(timeRTC->Minute + NIGHT_SLEEP_FOR_M, true);
    }
    else
    {
      //debugLog("Next minute wake up");
      SRTC.nextMinuteWake(true);
    }
  }
  else
  {
    debugLog("Not waking up, at all!");
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

String getHourMinute(tmElements_t *timeEl)
{
  //isDebug(dumpRTCTime(timeEl));
  String h = String(timeEl->Hour);
  if (h.length() == 1)
  {
    h = "0" + h;
  }

  String m = String(timeEl->Minute);
  if (m.length() == 1)
  {
    m = "0" + m;
  }

  String answer = h + ":" + m;
  debugLog("Answer: " + answer);
  return answer;
}

String getDayName(int offset)
{
  long unixTime = SRTC.doMakeTime(*timeRTC);
  int weekDay = weekday(unixTime);
  debugLog("unixTime: " + String(unixTime));
  debugLog("weekDay reported: " + String(weekDay));
  weekDay = weekDay + offset;
  checkMaxMin(&weekDay, 7, 1);
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

long getUnixTime()
{
  return SRTC.doMakeTime(*timeRTC);
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
  case 255:
  {
    // How?
    timeRTC->Month = 0;
    return "Jan";
  }
  default:
    return "Inv";
  }
}

String getFormattedTime(time_t rawTime) {
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

String unixToDate(unsigned long unixTime) {
  int gotDay = day(unixTime);
  int gotMonth = month(unixTime);
  int gotYear = year(unixTime);

  char dateString[11];
  sprintf(dateString, "%02d.%02d.%04d", gotDay, gotMonth, gotYear);
  
  return String(dateString);
}

long getHourDifference(time_t currentTime, time_t targetTime) {
  return abs((targetTime - currentTime) / 3600);
}

#if DEBUG
void initRTCDebug()
{
  debugLog("Get RTC battery level: " + String(SRTC.getRTCBattery(false)));
  debugLog("Get critical RTC battery level: " + String(SRTC.getRTCBattery(true)));
  debugLog("Get RTC type used: " + String(SRTC.getType()));
}

void loopRTCDebug()
{
  debugLog("SRTC.isOperating: " + BOOL_STR(SRTC.isOperating()));
  debugLog("SRTC.getADCPin: " + String(SRTC.getADCPin()));
  debugLog("SRTC.temperature(): " + String(SRTC.temperature()));
}

void dumpRTCTime(tmElements_t *timeEl)
{
  debugLog("Second: " + String(timeEl->Second));
  debugLog("Minute: " + String(timeEl->Minute));
  debugLog("Hour: " + String(timeEl->Hour));
  debugLog("Day: " + String(timeEl->Day));
  debugLog("Month: " + String(timeEl->Month));
  debugLog("Year: " + String(timeEl->Year));
}

void dumpRTCTimeSmall(tmElements_t *timeEl)
{
  debugLog("Time: " + String(timeEl->Minute) + ":" + String(timeEl->Second));
}
#endif
