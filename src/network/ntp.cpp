#include "ntp.h"

RTC_DATA_ATTR time_t previousNTPTimeFull = 0;      // Full latest NTP sync time
RTC_DATA_ATTR float previousNTPTimeDifference = 0; // The delay between 2 previous NTP syncs, float because of division of bigger delays
RTC_DATA_ATTR int ntpDriftCorrection = 0;          // The drift
RTC_DATA_ATTR float previousNTPCorrection = 0;     // UNIX time of previous drift correction, float because of division of bigger delays

bool firstNTPSync = true;
time_t initialRTCTime = 0;
int ntpTries = 0;

void syncNtp(bool doDriftThings)
{
    debugLog("Running syncNtp");
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);
    timeClient.begin();
    if (timeClient.update() == true)
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

        if (doDriftThings == true)
        {
            if (previousNTPTimeFull == 0)
            {
                previousNTPTimeFull = epochTime;
                debugLog("Setting initial ntp time for drift");
            }
            else
            {
                debugLog("Drift calc - NTP time: " + getFormattedTime(epochTime));
                debugLog("Drift calc - RTC time: " + getFormattedTime(currentTime));

                ntpDriftCorrection = currentTime - epochTime;
                previousNTPTimeDifference = epochTime - previousNTPTimeFull;
                if (previousNTPTimeDifference < VALID_PREVIOUS_SYNC_DELAY)
                {
                    debugLog("The delay is too small, rejecting...");
                    ntpDriftCorrection = 0;
                    previousNTPTimeDifference = 0;
                }
                else
                {

                    manageDriftTiming(&previousNTPTimeDifference, &ntpDriftCorrection);

                    debugLog("Drift calc - ntpDriftCorrection:" + String(ntpDriftCorrection));

                    previousNTPTimeFull = epochTime;
                    previousNTPCorrection = epochTime;
                }
            }
        }

        if(doDriftThings == false) {
            previousNTPCorrection = epochTime;
        }
        timeClient.end();
        wakeUpManageRTC(); // After syncing time, remake the alarm
    }
    else
    {
        debugLog("NTP error");
        if (ntpTries < 10)
        {
            ntpTries = ntpTries + 1;
            syncNtp();
        }
    }
}

void checkDrift()
{
    time_t currentTime = SRTC.doMakeTime(*timeRTC);
    if (currentTime < 0)
    {
        debugLog("SOMETHING IS WRONG :(");
        readRTC();
        currentTime = SRTC.doMakeTime(*timeRTC);
    }
    debugLog("currentTime: " + String(currentTime));
    debugLog("previousNTPCorrection: " + String(previousNTPCorrection));
    debugLog("previousNTPTimeDifference: " + String(previousNTPTimeDifference));
    int count = 0;
    // Checker
    int checker = (float(currentTime) - previousNTPCorrection) / previousNTPTimeDifference;
    debugLog("Checker is: " + String(checker));
    if (int(previousNTPCorrection) != 0 && int(ntpDriftCorrection) != 0 && checker > 2000)
    {
        debugLog("Checker is too high, do nothing");
        while (true)
        {
            delay(1000);
        }
    }
    bool changed = false;
    while (previousNTPCorrection != 0 && ntpDriftCorrection != 0 && abs(float(currentTime) - previousNTPCorrection) >= previousNTPTimeDifference)
    {
        previousNTPCorrection = previousNTPCorrection + previousNTPTimeDifference;
        previousNTPCorrection = precision(previousNTPCorrection, 3);
        debugLog("Doing drift correction!");
        debugLog("ntpDriftCorrection: " + String(ntpDriftCorrection));
        debugLog("previousNTPCorrection: " + String(previousNTPCorrection));
        debugLog("previousNTPTimeDifference: " + String(previousNTPTimeDifference));
        currentTime = currentTime - ntpDriftCorrection; // Not sure
        debugLog("currentTime: " + String(currentTime));
        changed = true;
    }
    if (changed == true)
    {
        SRTC.doBreakTime(currentTime, *timeRTC);
        saveRTC();
        wakeUpManageRTC();
    }
}

#define MAX_TRIES 200
void manageDriftTiming(float *timeDifference, int *drift)
{
    float timeDifferenceVar = *timeDifference;
    int driftVar = *drift;

    bool done = false;
    for (int d = 2; d < MAX_TRIES; d++)
    {
        time_t newDrift = driftVar / d;
        float newTimeDifference = timeDifferenceVar / d;

        if (newTimeDifference <= REPAIR_TIME_S && newDrift * d == driftVar)
        {
            debugLog("Stopped at divison: " + String(d));
            debugLog("old drift: " + String(driftVar));
            debugLog("newDrift: " + String(newDrift));
            debugLog("old difference: " + String(timeDifferenceVar));
            debugLog("newTimeDifference: " + String(newTimeDifference));
            driftVar = newDrift;
            newTimeDifference = precision(newTimeDifference, 3);
            timeDifferenceVar = newTimeDifference;
            done = true;
            break;
        }
    }

    if (done == false)
    {
        debugLog("FAILED to minimase drift :(");
        return;
    }

    *timeDifference = timeDifferenceVar;
    *drift = driftVar;
}