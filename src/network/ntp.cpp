#include "ntp.h"

bool firstNTPSync = true;
time_t initialRTCTime = 0;
int ntpTries = 0;

#if TIME_DRIFT_CORRECTION
RTC_DATA_ATTR uint64_t driftStartUnix = 0;
#endif

void syncNtp(bool doDriftThings)
{
    debugLog("Running syncNtp");
    dontTouchTimeZone = true;
    removeTimeZoneVars();
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);
#if MANUAL_NTP_OFFSET != 0
    timeClient.setTimeOffset(MANUAL_NTP_OFFSET);
#endif
    timeClient.begin();
    if (timeClient.forceUpdate() == true)
    {
        // timeClient.setTimeOffset(TIME_OFFSET_S);
        time_t epochTime = timeClient.getEpochTime();
        if (firstNTPSync == true)
        {
            debugLog("firstNTPSync is true, running again");
            firstNTPSync = false;
            initialRTCTime = epochTime;
            timeClient.end();
            syncNtp(doDriftThings);
            return;
        }
        else
        {
            // It runs it 2 times then compares the time
            int difference = abs(epochTime - initialRTCTime);
            debugLog("difference: " + String(difference));
            if (difference > 30)
            {
                debugLog("Difference too high, running ntp once more");
                initialRTCTime = epochTime;
                timeClient.end();
                syncNtp(doDriftThings);
                return;
            }
        }
        debugLog("NTP success");
        debugLog("NTP time: " + timeClient.getFormattedTime());
        debugLog("NTP unix time: " + String(epochTime));
        debugLog("millis are: " + String(millisBetter()));

        /* GuruSR:
        SmallRTC doesn't cause time to drift.  The drift values are all reset on init, they never change anything when like that.  Are you using the SRTC's make and break time functions, if not, that is what is happening, the standard makeTime and breakTime are not compliant to time.h, they add month and day onto the values.  With the correct TimeZone (TZ) used, using localTime against the RTC having UTC values, you'll get accurate DST calculations.

        I haven't looked at your code, but the make and break Time functions outside of SmallRTC don't follow time.h for values, so the day and month will increase.
        */

        SRTC.doBreakTime(epochTime, timeRTCUTC0);
        saveRTC(timeRTCUTC0);
        debugLog("Reading rtc from ntp");
        dontTouchTimeZone = false;
        readRTC(); // After syncing time, remake the timezone
        // Reset
        firstNTPSync = true;
        ntpTries = 0;
#if TIME_DRIFT_CORRECTION
        if (doDriftThings == true)
        {
            if (SRTC.checkingDrift() == true && (getUnixTime(timeRTC) - driftStartUnix > TIME_DRIFT_MINIMUM_TIME * 3600 || driftStartUnix == 0))
            {
                debugLog("Ending drift");
                SRTC.endDrift(timeRTCLocal);
                driftStartUnix = 0;
                uint32_t driftValue = SRTC.getDrift();
                bool driftIsFast = SRTC.isFastDrift();
                debugLog("isFast: " + String(driftIsFast) + " drift value: " + String(driftValue));
                fsSetString(CONF_DRIFT, String(driftValue));
                fsSetString(CONF_DRIFT_FAST, String(driftIsFast));
            }
            else
            {
                debugLog("Beginning new drift");
                // Drift is not going on or it's going on to quick to end it
                SRTC.beginDrift(timeRTCLocal);
                driftStartUnix = getUnixTime(timeRTCLocal);
            }
        } else {
            debugLog("Canceling drift");
            SRTC.setDrift(0, 0);
        }
#endif

        timeClient.end();
    }
    else
    {
        debugLog("NTP error");
        timeClient.end();
        if (ntpTries < 10)
        {
            ntpTries = ntpTries + 1;
            syncNtp(doDriftThings);
            return;
        }
    }
    dontTouchTimeZone = false;
}