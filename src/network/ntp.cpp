#include "ntp.h"

bool firstNTPSync = true;
time_t initialRTCTime = 0;
int ntpTries = 0;

void syncNtp()
{
    debugLog("Running syncNtp");
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);
    timeClient.begin();
    if (timeClient.forceUpdate() == true)
    {
        timeClient.setTimeOffset(TIME_OFFSET_S);
        time_t epochTime = timeClient.getEpochTime();
        SRTC.read(*timeRTC);
        time_t currentTime = SRTC.doMakeTime(*timeRTC); // Save the time to a var and update it afterwards for better accuracy
        if (firstNTPSync == true)
        {
            debugLog("firstNTPSync is true, running again");
            firstNTPSync = false;
            initialRTCTime = currentTime;
            SRTC.doBreakTime(epochTime, *timeRTC);
            saveRTC();
            timeClient.end();
            syncNtp();
            return;
        }
        else
        {
            // It runs it 2 times then compares the time
            int difference = abs(epochTime - currentTime);
            debugLog("difference: " + String(difference));
            if (difference > 5)
            {
                debugLog("Difference too high, running ntp once more");
                SRTC.doBreakTime(epochTime, *timeRTC);
                saveRTC();
                timeClient.end();
                syncNtp();
                return;
            }
        }
        debugLog("NTP success");
        debugLog("NTP time: " + timeClient.getFormattedTime());
        currentTime = initialRTCTime;

        /* GuruSR:
        SmallRTC doesn't cause time to drift.  The drift values are all reset on init, they never change anything when like that.  Are you using the SRTC's make and break time functions, if not, that is what is happening, the standard makeTime and breakTime are not compliant to time.h, they add month and day onto the values.  With the correct TimeZone (TZ) used, using localTime against the RTC having UTC values, you'll get accurate DST calculations.

        I haven't looked at your code, but the make and break Time functions outside of SmallRTC don't follow time.h for values, so the day and month will increase.
        */

        SRTC.doBreakTime(epochTime, *timeRTC);
        saveRTC();

        timeClient.end();
        wakeUpManageRTC(); // After syncing time, remake the alarm
    }
    else
    {
        debugLog("NTP error");
        timeClient.end();
        if (ntpTries < 10)
        {
            ntpTries = ntpTries + 1;
            syncNtp();
        }
    }
}