#include "rtc.h"
#include "rtcMem.h"

tmElements_t timeRTCLocal;         // Local time
tmElements_t timeRTCUTC0;          // UTC0 time
int64_t timeZoneOffset = 0;        // The offset the timezone did, can be in minus
uint64_t lastTimeRead = 999999999; // Millis of latest reading of the RTC, it's used to get accurate seconds, it's that much to trigger the alarm wakeup if something fails, llabs is there for this reason

void initRTC()
{
  timeRTCLocal = {};
  timeRTCUTC0 = {};
#if RTC_TYPE == EXTERNAL_RTC
  pinMode(RTC_INT_PIN, INPUT);
#endif
  if (bootStatus.fromWakeup == false)
  {
#if RTC_TYPE == INTERNAL_RTC
    rM.SRTC.useESP32(true, RTC_32KHZ_CRYSTAL);
#endif
    rM.SRTC.init();
    // I don't trust it
#if RTC_TYPE == INTERNAL_RTC
    rM.SRTC.useESP32(true, RTC_32KHZ_CRYSTAL);
#endif

#if RESET_RTC_ON_BOOT && DEBUG
    if (bootStatus.fromWakeup == false)
    {
      rM.SRTC.rtc_pcf.initClock();
    }
#endif

#if TIME_DRIFT_CORRECTION
    if (rM.SRTC.getDrift() == 0)
    {
      uint32_t driftValue = fsGetString(CONF_DRIFT, "0").toInt();
      if (driftValue != 0)
      {
        debugLog("Setting initial drift value, as it's not set and the watchy is not from wakeup");
        bool isFast = bool(fsGetString(CONF_DRIFT_FAST, "0").toInt());
        debugLog("isFast: " + String(isFast) + " drift value: " + String(driftValue));
        rM.SRTC.setDrift(driftValue, isFast); // not sure about internal here
      }
      else
      {
        debugLog("Something is wrong with drift value, probably. Or it's not set :)");
      }
    }
#endif
  }

  debugLog("Using internal RTC clock: " + BOOL_STR(rM.SRTC.onESP32()));

  readRTC();
  setupMillisComparators();

#if DEBUG && RTC_TYPE == INTERNAL_RTC
  // Should be 1 for external RTC quartz, if it's 0 then it's using the internal RTC which is not accurate
  // It's set here: https://github.com/espressif/esp-idf/blob/dbce23f8a449eb436b0b574726fe6ce9a6df67cc/components/esp_system/port/soc/esp32c6/clk.c#L179
  debugLog("Internal RTC source clock: " + String(rtc_clk_slow_src_get()));

  // Some further testing:
  // Go to (Adjust path for soc) /root/.platformio/packages/framework-espidf/components/esp_system/port/soc/esp32c6/clk.c and at line arround 195
  // ESP_EARLY_LOGD(TAG, "RTC_SLOW_CLK calibration value: %" PRIu32, cal_val);
  // Change ESP_EARLY_LOGD to ESP_EARLY_LOGE for the value to appear
  // If you can't catch this log, edit the line ESP_EARLY_LOGD(TAG, "waiting for 32k oscillator to start up"); to
  /*
  for(int i = 0; i < 300; i++) {
    ESP_EARLY_LOGE(TAG, "waiting for 32k oscillator to start up");
  }
  */
  // The value should be arround, 16000000 because, explanation:
  // This function, select_rtc_slow_clk and the while loop compares how many cycles pass for the 32Khz quartz in x ammount of time (cpu cycles) and compares it to the 160Khz internal clock, which it assumes is accurate to a degree.
  // So for example:
  // 16000593 is fine
  // 15999959 for a 20 ppm clock is good
  // 15999701 for 5 ppm clock, I gues the check above was pure luck, this result is still good
  // 15999697 also 5 ppm clock
  // 15999699 also 5 ppm clock - It's the most reliable one (The 20 ppm above must be pure luck)
  // 15997884 for watchy v3 lol

  // Some more logs I gathered:
  /*
  D (1705) clk: waiting for 32k oscillator to start up
  D (1710) clk: clk 32 enable
  D (1713) clk: calibrate slow clk
  W (1716) rtc_time: clk_ll_32k_calibration_set_target
  W (1720) rtc_time: expected_freq: 32768
  W (1724) rtc_time: us_time_estimate: 91552
  W (1819) rtc_time: return cal_val: 3662245
  W (1820) rtc_time: xtal_cycles: 1082652004
  W (1820) clk: rtc_slow_clk_src: 1
  W (1821) rtc_time: clk_ll_32k_calibration_set_target
  W (1823) rtc_time: expected_freq: 32768
  W (1827) rtc_time: us_time_estimate: 91552
  W (1922) rtc_time: return cal_val: 3662245
  W (1923) rtc_time: xtal_cycles: 1082652004
  W (1923) clk: cal_val in todo: 16000593
  D (1924) clk: RTC_SLOW_CLK calibration value: 16000593
  */
  // Links:
  // https://github.com/espressif/esp-idf/issues/11755

  /*
  Further more, to check if the quartz works (but faster, when soldering)
  /root/.platformio/packages/framework-espidf/components/esp_system/port/soc/esp32c6/clk.c
  in line 180, replace this log: ESP_EARLY_LOGW(TAG, "32 kHz clock not found, switching to internal 150 kHz oscillator");
  with:
  while(true) {
    ESP_EARLY_LOGE(TAG, "32 kHz clock not found, switching to internal 150 kHz oscillator");
  }

  Also change other logs to Errors, so they are visible
  */
#endif
}

// Make sure you save bare UTC0 time here, no timezone
void saveRTC(tmElements_t timeToSave)
{
  debugLog("Saving time to RTC: " + String(getUnixTime(timeToSave)));
  rM.SRTC.set(timeToSave);

#if DEBUG
  tmElements_t test;
  rM.SRTC.read(test);
  debugLog("Readed time back: " + String(getUnixTime(test)));
#endif
}

void setRTCTimeUnix(time_t epoch)
{
  dontTouchTimeZone = true;
  rM.SRTC.doBreakTime(epoch, timeRTCUTC0);
  saveRTC(timeRTCUTC0);
  dontTouchTimeZone = false;
}

void setRTCTimeZoneByUtcOffset(int offset)
{
  // Flip the sign because that's how posix timezones are defined.
  // Use GMT for now, which will create a static timezone,
  // meaning we won't account for daylight saving time.
  String tz = "GMT" + String(offset * -1);
  debugLog("Setting timezone to: " + tz);
  strncpy(rM.posixTimeZone, tz.c_str(), tz.length());
  rM.posixTimeZone[tz.length()] = '\0';
  readRTC();
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
// If you are setting time, turn this to true, then back to false
bool dontTouchTimeZone = false;
void timeZoneApply()
{
  if (dontTouchTimeZone == true)
  {
    debugLog("Don't touch timezone is on");
    return;
  }
  timeRTCLocal = timeRTCUTC0; // To have at least UTC0 there

  // https://github.com/Michal-Szczepaniak/TinyWatchy/commit/cb9082fe0f8df6ac4dc3ff682a7ddc80ef07d78f
  // https://docs.espressif.com/projects/esp-idf/en/v5.1.4/esp32/api-reference/system/system_time.html
  if (strlen(rM.posixTimeZone) > 0)
  {

#if TIME_ZONE_DUMP
    debugLog("Before timezone:");
    debugLog("seconds: " + String(timeRTCLocal.Second));
    debugLog("minutes: " + String(timeRTCLocal.Minute));
    debugLog("hours: " + String(timeRTCLocal.Hour));
    debugLog("day: " + String(timeRTCLocal.Day));
    debugLog("month: " + String(timeRTCLocal.Month));
    debugLog("day of the week: " + String(timeRTCLocal.Wday));
    debugLog("year: " + String(timeRTCLocal.Year));
#endif
    int64_t initialUnixTime = getUnixTime(timeRTCUTC0);
    // https://man7.org/linux/man-pages/man3/setenv.3.html
    const char *currentTZ = getenv("TZ");
    bool isItSet = (currentTZ != nullptr && strcmp(currentTZ, rM.posixTimeZone) != 0);
    if (isItSet || setenv("TZ", rM.posixTimeZone, 1) == 0)
    {
      if (isItSet == false)
      {
        tzset();
      }
      debugLog("rM.posixTimeZone: " + String(rM.posixTimeZone));
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

      timeRTCLocal = convertToTmElements(tempTM);
      time_t timeZoneUnix = getUnixTime(timeRTCLocal);

      timeZoneOffset = initialUnixTime - timeZoneUnix;
      debugLog("Unix times: " + String(initialUnixTime) + " " + String(timeZoneUnix) + " " + String(initialUnixTime - timeZoneUnix));

#if TIME_ZONE_DUMP
      debugLog("After timezone:");
      debugLog("seconds: " + String(timeRTCLocal.Second));
      debugLog("minutes: " + String(timeRTCLocal.Minute));
      debugLog("hours: " + String(timeRTCLocal.Hour));
      debugLog("day: " + String(timeRTCLocal.Day));
      debugLog("month: " + String(timeRTCLocal.Month));
      debugLog("day of the week: " + String(timeRTCLocal.Wday));
      debugLog("year: " + String(timeRTCLocal.Year));
#endif
      // debugLog("Timezone set succes, current timezone: " + String(rM.posixTimeZone));
      debugLog("Timezone working");
    }
    else
    {
      debugLog("Failed to set posix timezone");
    }
    // This causes a memory leak???????????
    // removeTimeZoneVars();
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
        strncpy(rM.posixTimeZone, posix.c_str(), posix.length());
        rM.posixTimeZone[posix.length()] = '\0';
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

void removeTimeZoneVars()
{
  debugLog("Removing timezone variables");
  unsetenv("TZ");
  tzset();
}

void readRTC()
{
  // debugLog("Reading RTC");
  rM.SRTC.read(timeRTCUTC0);
  lastTimeRead = millisBetter();
  debugLog("Time retrieved from RTC: " + String(getUnixTime(timeRTCUTC0)));

#if RTC_TYPE == INTERNAL_RTC
  bool rtcGarbage = false;
  if (timeRTCUTC0.Year < 50 || timeRTCUTC0.Year > 100)
  {
    timeRTCUTC0.Year = 54;
    rtcGarbage = true;
  }
  if (timeRTCUTC0.Month > 11)
  {
    timeRTCUTC0.Month = 0;
    rtcGarbage = true;
  }
  if (timeRTCUTC0.Day > 31)
  {
    timeRTCUTC0.Day = 0;
    rtcGarbage = true;
  }
  if (timeRTCUTC0.Hour > 24)
  {
    timeRTCUTC0.Hour = 0;
    rtcGarbage = true;
  }
  if (timeRTCUTC0.Minute > 60)
  {
    timeRTCUTC0.Minute = 0;
    rtcGarbage = true;
  }
  if (rtcGarbage == true)
  {
    debugLog("RTC garbage, repaired");
    saveRTC(timeRTCUTC0);
  }
#endif

  timeZoneApply();
}

void wakeUpIn(int minutes)
{
  if (minutes < 1)
  {
    debugLog("Wake up in minutes below 1, never waking up");
    return;
  }
  if (minutes != 1)
  {
    debugLog("Next wake up in " + String(minutes) + " minutes");
    // isDebug(dumpRTCTime());
    uint hour = timeRTCLocal.Hour;
    int fullMinutes = int((hour * 60) + timeRTCLocal.Minute + minutes) + (timeZoneOffset / 60);
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

    rM.SRTC.atTimeWake(finalHour, finalMinute, true);
  }
  else
  {
    debugLog("Next minute wake up");
    rM.SRTC.nextMinuteWake(true);
  }
}

void wakeUpManageRTC()
{
  // debugLog("Launching wakeUpManageRTC");
  rM.SRTC.clearAlarm();
  int minutes = 0;
  if (rM.disableWakeUp == false)
  {
    // Not needed as we check if we go before next minute to sleep
    // readRTC();
    // isDebug(dumpRTCTime());
    uint hour = timeRTCLocal.Hour;
    // debugLog("timeRTCLocal.Hour: " + String(hour));
    if (NIGHT_SLEEP_FOR_M != 1 && (hour >= NIGHT_SLEEP_AFTER_HOUR || hour < NIGHT_SLEEP_BEFORE_HOUR))
    {
      minutes = NIGHT_SLEEP_FOR_M;
    }
    else
    {
      minutes = 1;
    }
  }
  else
  {
    debugLog("Not waking up, at all!");
  }

#if INK_ALARMS
  if (rM.nextAlarm == 0)
  {
    calculateNextAlarm();
  }
  if (rM.nextAlarm != 0)
  {
    uint64_t unixTimeNow = getUnixTime(timeRTCLocal);
    uint32_t minutesTillAlarm = (rM.nextAlarm - unixTimeNow) / 60;
    debugLog("unixTimeNow: " + String(unixTimeNow));
    debugLog("rM.nextAlarm: " + String(rM.nextAlarm));
    debugLog("minutesTillAlarm: " + String(minutesTillAlarm));
    if (minutesTillAlarm < minutes || minutes == 0)
    {
      debugLog("Minutes till alarm is lower, replacing!");
      minutes = minutesTillAlarm;
    }
  }
#endif

  wakeUpIn(minutes);
}

uint8_t manageRtcLastSec = 0;
void manageRTC()
{
  // debugLog("Executed manageRTC");
  // #if RTC_TYPE == EXTERNAL_RTC
  //   if (digitalRead(RTC_INT_PIN) == LOW)
  // #elif RTC_TYPE == INTERNAL_RTC

  // #endif

  uint8_t newSec = getCurrentSeconds();
  if (newSec != manageRtcLastSec)
  {
    manageRtcLastSec = newSec;
    debugLog("getCurrentSeconds(): " + String(getCurrentSeconds()));
    if (manageRtcLastSec <= 1 || getLastTimeReadSec() >= 60)
    {
      debugLog("RTC new minute");
      loopBattery();
      readRTC();
    }
  }
}

String getHourMinuteUnix(int64_t unixTime)
{
  tmElements_t tmEl;
  rM.SRTC.doBreakTime(unixTime, tmEl);
  return getHourMinute(tmEl);
}

String getHourMinute(tmElements_t timeEl)
{
  // isDebug(dumpRTCTime(timeEl));
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

  // debugLog("The bare hour: " + String(timeEl.Hour) + " and the bare minute: " + String(timeEl.Minute));
  String answer = h + ":" + m;
  // debugLog("Answer: " + answer);
  return answer;
}

String unixToDayName(uint64_t unixTime, int offset)
{
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

String getDayName(int offset)
{
  long unixTime = rM.SRTC.doMakeTime(timeRTCLocal);
  return unixToDayName(unixTime, offset);
}

uint64_t getUnixTime(tmElements_t tme)
{
  return rM.SRTC.doMakeTime(tme);
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
    debugLog("We have a problem month 255");
    return "Jan";
  }
  default:
    return "Inv";
  }
}

String getMonthNameFull(int monthNumber)
{
  debugLog("Month given: " + String(monthNumber));
  switch (monthNumber)
  {
  case 0:
    return "January";
  case 1:
    return "February";
  case 2:
    return "March";
  case 3:
    return "April";
  case 4:
    return "May";
  case 5:
    return "June";
  case 6:
    return "July";
  case 7:
    return "August";
  case 8:
    return "September";
  case 9:
    return "October";
  case 10:
    return "November";
  case 11:
    return "December";
  case 255:
  {
    // Handle the special case for month 255
    debugLog("We have a problem month 255");
    return "January"; // or any other default month you prefer
  }
  default:
    return "Invalid"; // Changed from "Inv" to "Invalid" for clarity
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
  debugLog("Get RTC battery level: " + String(rM.SRTC.getRTCBattery(false)));
  debugLog("Get critical RTC battery level: " + String(rM.SRTC.getRTCBattery(true)));
  debugLog("Get RTC type used: " + String(rM.SRTC.getType()));
}

void loopRTCDebug()
{
  debugLog("rM.SRTC.isOperating: " + BOOL_STR(rM.SRTC.isOperating()));
  debugLog("rM.SRTC.getADCPin: " + String(rM.SRTC.getADCPin()));
  debugLog("rM.SRTC.temperature(): " + String(rM.SRTC.temperature()));
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
  debugLog("Time: " + String(timeEl.Hour) + ":" + String(timeEl.Minute) + ":" + String(timeEl.Second));
}
#endif

// It is in fact needed
// https://github.com/espressif/esp-idf/blob/5ca9f2a49aaabbfaf726da1cc3597c0edb3c4d37/components/newlib/port/esp_time_impl.c#L138
// Ok maybe not, I'm really confused at this point
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
  uint currentSeconds = (getLastTimeReadSec() + timeRTCLocal.Second) % 60;
  return currentSeconds;
}
