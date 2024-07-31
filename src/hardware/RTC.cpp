#include "RTC.h"

tmElements_t timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

RTC_DATA_ATTR char posixTimeZone[POSIX_TIMEZONE_MAX_LENGTH] = TIMEZONE_POSIX;

int64_t timeZoneOffset = 0;

// Millis of latest reading of the RTC
uint64_t lastTimeRead = 999999; // it's that much to trigger the alarm wakeup if something fails, llabs is there for this reason

void initRTC(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
  timeRTC = {};
#if RTC_TYPE == EXTERNAL_RTC
  pinMode(RTC_INT_PIN, INPUT);
#endif
  if (isFromWakeUp == false)
  {
#if RTC_TYPE == INTERNAL_RTC
    SRTC.useESP32(true, RTC_32KHZ_CRYSTAL);
#endif
    SRTC.init();
    // I don't trust it
#if RTC_TYPE == INTERNAL_RTC
    SRTC.useESP32(true, RTC_32KHZ_CRYSTAL);
#endif

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

  debugLog("Using internal RTC clock: " + BOOL_STR(SRTC.onESP32()));

  readRTC();
  setupMillisComparators();
}

// Make sure you save bare UTC0 time here, no timezone
void saveRTC(tmElements_t timeToSave)
{
  SRTC.set(timeToSave);

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

tmElements_t convertToTmElements(const struct tm &tmStruct)
{
  tmElements_t elements;
  // because mktime sucks UTC and timegm doesn't exist
  elements.Second = static_cast<uint8_t>(tmStruct.tm_sec);
  elements.Minute = static_cast<uint8_t>(tmStruct.tm_min);
  elements.Hour = static_cast<uint8_t>(tmStruct.tm_hour);
  elements.Wday = static_cast<uint8_t>(tmStruct.tm_wday + 1); // Adjust to start from Sunday = 1
  elements.Day = static_cast<uint8_t>(tmStruct.tm_mday);
  elements.Month = static_cast<uint8_t>(tmStruct.tm_mon);
  elements.Year = static_cast<uint8_t>(tmStruct.tm_year - 70); // Offset from 1970

  return elements;
}

#define TIME_ZONE_DUMP 0
void timeZoneApply()
{
  // https://github.com/Michal-Szczepaniak/TinyWatchy/commit/cb9082fe0f8df6ac4dc3ff682a7ddc80ef07d78f
  // https://docs.espressif.com/projects/esp-idf/en/v5.1.4/esp32/api-reference/system/system_time.html
  if (strlen(posixTimeZone) > 0)
  {

#if TIME_ZONE_DUMP
    debugLog("Before timezone:");
    debugLog("seconds: " + String(timeRTC.Second));
    debugLog("minutes: " + String(timeRTC.Minute));
    debugLog("hours: " + String(timeRTC.Hour));
    debugLog("day: " + String(timeRTC.Day));
    debugLog("month: " + String(timeRTC.Month));
    debugLog("day of the week: " + String(timeRTC.Wday));
    debugLog("year: " + String(timeRTC.Year));
#endif

    // https://man7.org/linux/man-pages/man3/setenv.3.html
    if (setenv("TZ", posixTimeZone, 1) == 0)
    {
      tzset();
      int64_t initialUnixTime = getUnixTime();
      time_t tempTime = initialUnixTime;
      struct tm tempTM = {};
      localtime_r(&tempTime, &tempTM);
#if TIME_ZONE_DUMP
      debugLog("tempTM->tm_sec: " + String(tempTM.tm_sec));
      debugLog("tempTM->tm_min: " + String(tempTM.tm_min));
      debugLog("tempTM->tm_hour: " + String(tempTM.tm_hour));
      debugLog("tempTM->tm_mday: " + String(tempTM.tm_mday));
      debugLog("tempTM->tm_mon: " + String(tempTM.tm_mon));
      debugLog("tempTM->tm_year: " + String(tempTM.tm_year));
      debugLog("tempTM->tm_wday: " + String(tempTM.tm_wday));
      debugLog("tempTM->tm_yday: " + String(tempTM.tm_yday));
      debugLog("tempTM->tm_isdst: " + String(tempTM.tm_isdst));
#endif

      timeRTC = convertToTmElements(tempTM);
      time_t timeZoneUnix = getUnixTime();

      timeZoneOffset = initialUnixTime - timeZoneUnix;
      debugLog("Unix times: " + String(initialUnixTime) + " " + String(timeZoneUnix) + " " + String(initialUnixTime - timeZoneUnix));

#if TIME_ZONE_DUMP
      debugLog("After timezone:");
      debugLog("seconds: " + String(timeRTC.Second));
      debugLog("minutes: " + String(timeRTC.Minute));
      debugLog("hours: " + String(timeRTC.Hour));
      debugLog("day: " + String(timeRTC.Day));
      debugLog("month: " + String(timeRTC.Month));
      debugLog("day of the week: " + String(timeRTC.Wday));
      debugLog("year: " + String(timeRTC.Year));
#endif
      // debugLog("Timezone set succes, current timezone: " + String(posixTimeZone));
      debugLog("Timezone working");
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
        posixTimeZone[posix.length()] = '\0';
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
  SRTC.read(timeRTC);

#if RTC_TYPE == INTERNAL_RTC
  bool rtcGarbage = false;
  if (timeRTC.Year < 50 || timeRTC.Year > 100)
  {
    timeRTC.Year = 54;
    rtcGarbage = true;
  }
  if (timeRTC.Month > 11)
  {
    timeRTC.Month = 0;
    rtcGarbage = true;
  }
  if (timeRTC.Day > 31)
  {
    timeRTC.Day = 0;
    rtcGarbage = true;
  }
  if (timeRTC.Hour > 24)
  {
    timeRTC.Hour = 0;
    rtcGarbage = true;
  }
  if (timeRTC.Minute > 60)
  {
    timeRTC.Minute = 0;
    rtcGarbage = true;
  }
  if (rtcGarbage == true)
  {
    debugLog("RTC garbage, repaired");
    saveRTC(timeRTC);
  }
#endif

  timeZoneApply(); // Here because of garbage cleaning
  lastTimeRead = millisBetter();
}

void wakeUpManageRTC()
{
  SRTC.clearAlarm();
  if (disableWakeUp == false)
  {
    readRTC();
    // isDebug(dumpRTCTime());
    uint hour = timeRTC.Hour;
    // debugLog("timeRTC.Hour: " + String(hour));
    if (NIGHT_SLEEP_FOR_M != 1 && (hour >= NIGHT_SLEEP_AFTER_HOUR || hour < NIGHT_SLEEP_BEFORE_HOUR))
    {
      debugLog("Next wake up in " + String(NIGHT_SLEEP_FOR_M) + " minutes");
      // isDebug(dumpRTCTime());
      int fullMinutes = int((hour * 60) + timeRTC.Minute + NIGHT_SLEEP_FOR_M) + (timeZoneOffset / 60);
      debugLog("fullMinutes: " + String(fullMinutes));
      // Timezone triggered backwards, it's on minutes, add secconds
      if (fullMinutes < 0)
      {
        fullMinutes = 1440 + fullMinutes;
      }
      // 60 * 24 = 1440 so a full day in minutes
      if (fullMinutes >= (1440))
      {
        fullMinutes = fullMinutes - 1440;
      }
      uint finalHour = fullMinutes / 60; // Will always round up to the floor
      uint finalMinute = fullMinutes - (finalHour * 60);
      debugLog("The watch will wake up at exactly (in UTC0, taking in account your timezone) hour: " + String(finalHour) + " and minute: " + String(finalMinute));

      SRTC.atTimeWake(finalHour, finalMinute, true);
    }
    else
    {
      debugLog("Next minute wake up");
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
  // debugLog("Executed alarmManageRTC");
  // #if RTC_TYPE == EXTERNAL_RTC
  //   if (digitalRead(RTC_INT_PIN) == LOW)
  // #elif RTC_TYPE == INTERNAL_RTC

  // #endif

  if (getLastTimeReadSec() >= 60)
  {
    debugLog("RTC new minute");
    readRTC();
  }
}

String getHourMinute(tmElements_t timeEl)
{
  isDebug(dumpRTCTime(timeEl));
  String h = String(timeEl.Hour);
  if (h.length() == 1)
  {
    h = "0" + h;
  }

  String m = String(timeEl.Minute);
  if (m.length() == 1)
  {
    m = "0" + m;
  }

  debugLog("The bare hour: " + String(timeEl.Hour) + " and the bare minute: " + String(timeEl.Minute));
  String answer = h + ":" + m;
  debugLog("Answer: " + answer);
  return answer;
}

String getDayName(int offset)
{
  long unixTime = SRTC.doMakeTime(timeRTC);
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

uint64_t getUnixTime()
{
  return SRTC.doMakeTime(timeRTC);
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
    timeRTC.Month = 0;
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

String unixToDate(uint64_t unixTime)
{
  int gotDay = day(unixTime);
  int gotMonth = month(unixTime);
  int gotYear = year(unixTime);

  char dateString[11];
  sprintf(dateString, "%02d.%02d.%04d", gotDay, gotMonth, gotYear);

  return String(dateString);
}

uint64_t simplifyUnix(uint64_t unixTime)
{
  return unixTime - (unixTime % 86400);
  /*
  std::time_t time = static_cast<std::time_t>(unixTime);

  std::tm *date = std::localtime(&time);

  date->tm_hour = 0;
  date->tm_min = 0;
  date->tm_sec = 0;

  std::time_t simplified_time = std::mktime(date);

  return static_cast<uint64_t>(simplified_time);
  */
}

// Accepts format "%02d.%02d.%04d"
uint64_t dateToUnix(String date)
{
  int day, month, year;
  sscanf(date.c_str(), "%02d.%02d.%04d", &day, &month, &year);

  tmElements_t tm = {};
  tm.Year = year - 1970;
  tm.Month = month;
  tm.Day = day;

  uint64_t t = static_cast<uint64_t>(makeTime(tm));
  debugLog("For date: " + date + " returning unix time: " + String(t));
  return t;
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

void dumpRTCTime(tmElements_t timeEl)
{
  debugLog("Second: " + String(timeEl.Second));
  debugLog("Minute: " + String(timeEl.Minute));
  debugLog("Hour: " + String(timeEl.Hour));
  debugLog("Day: " + String(timeEl.Day));
  debugLog("Month: " + String(timeEl.Month));
  debugLog("Year: " + String(timeEl.Year));
}

void dumpRTCTimeSmall(tmElements_t timeEl)
{
  debugLog("Time: " + String(timeEl.Minute) + ":" + String(timeEl.Second));
}
#endif

// It is in fact needed
// https://github.com/espressif/esp-idf/blob/5ca9f2a49aaabbfaf726da1cc3597c0edb3c4d37/components/newlib/port/esp_time_impl.c#L138
void setupMillisComparators()
{
  // Every value that compares to millis needs to be set here, or if it's used only locally, like it's initialized every time then we don't need it
  uint64_t theMillis = millisBetter();
#if DEBUG
  loopDumpDelayMs = theMillis;
#endif
  sleepDelayMs = theMillis;
}

uint64_t getLastTimeReadSec()
{
  // To make it the upper without calling cell() here
  uint64_t lastTimeReadSec = (llabs(millisBetter() - lastTimeRead) + 999) / 1000;
  return lastTimeReadSec;
}

uint getCurrentSeconds()
{
  // debugLog("lastTimeReadSec: " + String(lastTimeReadSec));
  uint currentSeconds = (getLastTimeReadSec() + timeRTC.Second) % 60;
  return currentSeconds;
}