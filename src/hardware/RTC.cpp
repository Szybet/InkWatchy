#include "RTC.h"

tmElements_t *timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

RTC_DATA_ATTR char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH] = TIMEZONE_POSIX;

void setupTimeStructure()
{
  debugLog("Created time memory");
  timeRTC = new tmElements_t;
}

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
#if ATCHY_VER == WATCHY_2
  pinMode(RTC_INT_PIN, INPUT);
#endif
  if (isFromWakeUp == false)
  {
    SRTC.init();
    // Done inside smallrtc
    // #if RTC_INT_PIN == -1
    // SRTC.use32K(true);
    // #endif
    HWVer = SRTC.getWatchyHWVer();
#if TIME_DRIFT_CORRECTION
    if (SRTC.getDrift() == 0)
    {
      uint32_t driftValue = fsGetString(CONF_DRIFT, "0").toInt();
      if (driftValue != 0)
      {
        debugLog("Setting initial drift value, as it's not set and the watchy is not from wakeup");
        bool isFast = bool(fsGetString(CONF_DRIFT_FAST, "0").toInt());
        debugLog("isFast: " + String(isFast) + " drift value: " + String(driftValue));
        SRTC.setDrift(driftValue, isFast, false);
      }
      else
      {
        debugLog("Something is wrong with drift value, probably. Or it's not set :)");
      }
    }
#endif
  }
  if (wakeUpReason != RTC_WAKEUP_REASON)
  {
    setupTimeStructure();
    readRTC();
    wakeUpManageRTC();
  }
#if ATCHY_VER == WATCHY_3
  else
  {
    readRTC(); // To update millis(); always
  }
  setupMillisComparators();
#endif
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

#define TIME_ZONE_DUMP 0
void timeZoneApply()
{
  // https://github.com/Michal-Szczepaniak/TinyWatchy/commit/cb9082fe0f8df6ac4dc3ff682a7ddc80ef07d78f
  if (strlen(posixTimeZone) > 0)
  {

#if TIME_ZONE_DUMP == 1
    debugLog("Before timezone:");
    debugLog("seconds: " + String(timeRTC->Second));
    debugLog("minutes: " + String(timeRTC->Minute));
    debugLog("hours: " + String(timeRTC->Hour));
    debugLog("day: " + String(timeRTC->Day));
    debugLog("month: " + String(timeRTC->Month));
    debugLog("day of the week: " + String(timeRTC->Wday));
    debugLog("year: " + String(timeRTC->Year));
#endif

    // https://man7.org/linux/man-pages/man3/setenv.3.html
    if (setenv("TZ", posixTimeZone, 1) == 0)
    {
      tzset();
      time_t tempTime = makeTime(*timeRTC);
      struct tm *tempTM = localtime(&tempTime);
      // Madness
      timeRTC->Second = tempTM->tm_sec;
      timeRTC->Minute = tempTM->tm_min;
      timeRTC->Hour = tempTM->tm_hour; 
      timeRTC->Day = tempTM->tm_mday - 1; // AAAAAA
      timeRTC->Month = tempTM->tm_mon + 1;
      timeRTC->Year = tempTM->tm_year - 70; // Really? m y g o d
      timeRTC->Wday = tempTM->tm_wday + 1;

#if TIME_ZONE_DUMP == 1
      debugLog("After timezone:");
      debugLog("seconds: " + String(timeRTC->Second));
      debugLog("minutes: " + String(timeRTC->Minute));
      debugLog("hours: " + String(timeRTC->Hour));
      debugLog("day: " + String(timeRTC->Day));
      debugLog("month: " + String(timeRTC->Month));
      debugLog("day of the week: " + String(timeRTC->Wday));
      debugLog("year: " + String(timeRTC->Year));
#endif
    }
    else
    {
      debugLog("Failed to set posix timezone");
    }
  }
  else
  {
    if (strlen(TIMEZONE_OLSON) > 0)
    {
      Olson2POSIX timezoneMagic;
      timezoneMagic.setOlsonTimeZone(String(TIMEZONE_OLSON));
      String posix = timezoneMagic.getCurrentPOSIX();
      debugLog("Got posix from olson: " + posix);
      uint8_t posixLength = posix.length();
      if (posixLength < POSIX_TIMEZONE_MAX_LENGTH)
      {
        strncpy(posixTimeZone, posix.c_str(), posix.length());
        posixTimeZone[posix.length() - 1] = '\0';
        timeZoneApply();
      }
      else
      {
        debugLog("How is your posix timezone so long? BUG BUG contact me");
      }
    }
    else
    {
      debugLog("No timezone set, anywhere yet (ntp should fix this log)");
    }
  }
}

void readRTC()
{
  // debugLog("Reading RTC");
  SRTC.read(*timeRTC);
  timeZoneApply();

#if ATCHY_VER == WATCHY_3
  bool rtcGarbage = false;
  if (timeRTC->Year < 50 || timeRTC->Year > 100)
  {
    timeRTC->Year = 54;
    rtcGarbage = true;
  }
  if (timeRTC->Month > 11)
  {
    timeRTC->Month = 0;
    rtcGarbage = true;
  }
  if (timeRTC->Day > 31)
  {
    timeRTC->Day = 0;
    rtcGarbage = true;
  }
  if (timeRTC->Hour > 24)
  {
    timeRTC->Hour = 0;
    rtcGarbage = true;
  }
  if (timeRTC->Minute > 60)
  {
    timeRTC->Minute = 0;
    rtcGarbage = true;
  }
#if DEBUG
  if (rtcGarbage == true)
  {
    debugLog("RTC garbage");
  }
#endif
#endif
}

void wakeUpManageRTC()
{
  SRTC.clearAlarm();
  if (disableWakeUp == false)
  {
    // isDebug(dumpRTCTime());
    int hour = timeRTC->Hour;
    // debugLog("timeRTC->Hour: " + String(hour));
    //  Watchy 2.0 has problems here... Idk?
    if (NIGHT_SLEEP_FOR_M != 1 && (hour >= NIGHT_SLEEP_AFTER_HOUR || hour < NIGHT_SLEEP_BEFORE_HOUR))
    {
      // debugLog("Next wake up in " + String(NIGHT_SLEEP_FOR_M) + " minutes");
      // isDebug(dumpRTCTime());
      SRTC.atMinuteWake(timeRTC->Minute + NIGHT_SLEEP_FOR_M, true);
    }
    else
    {
      // debugLog("Next minute wake up");
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
#if ATCHY_VER == WATCHY_2
  if (digitalRead(RTC_INT_PIN) == LOW)
#elif ATCHY_VER == WATCHY_3
  if (SRTC.isNewMinute() == true)
#endif
  {
    debugLog("RTC PIN IS HIGH");
    readRTC();
    wakeUpManageRTC();
  }
}

String getHourMinute(tmElements_t *timeEl)
{
  // isDebug(dumpRTCTime(timeEl));
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

  debugLog("The bare hour: " + String(timeEl->Hour) + " and the bare minute: " + String(timeEl->Minute));
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

String getFormattedTime(time_t rawTime)
{
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

String unixToDate(unsigned long unixTime)
{
  int gotDay = day(unixTime);
  int gotMonth = month(unixTime);
  int gotYear = year(unixTime);

  char dateString[11];
  sprintf(dateString, "%02d.%02d.%04d", gotDay, gotMonth, gotYear);

  return String(dateString);
}

long getHourDifference(time_t currentTime, time_t targetTime)
{
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

void setupMillisComparators()
{
  // Every value that compares to millis needs to be set here, or if it's used only locally, like it's initialized every time then we don't need it
  uint64_t theMillis = millis();
#if DEBUG
  loopDumpDelayMs = theMillis;
#endif
  sleepDelayMs = theMillis;
}