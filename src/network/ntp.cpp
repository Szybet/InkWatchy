#include "ntp.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

RTC_DATA_ATTR time_t previousNTPTimeFull = 0; // Full latest NTP sync time
RTC_DATA_ATTR time_t previousNTPTimeDifference = 0; // The delay between 2 previous NTP syncs 
RTC_DATA_ATTR int ntpDriftCorrection = 0; // The drift
RTC_DATA_ATTR time_t previousNTPCorrection = 0; // UNIX time of previous drift correction
#define ACCEPTABLE_DRIFT 15 // In S

void syncNtp() {
    timeClient.begin();
    if(timeClient.update() == true) {
        debugLog("NTP success");
        timeClient.setTimeOffset(TIME_OFFSET_S);
        debugLog("NTP time: " + timeClient.getFormattedTime());

        /* GuruSR:
        SmallRTC doesn't cause time to drift.  The drift values are all reset on init, they never change anything when like that.  Are you using the SRTC's make and break time functions, if not, that is what is happening, the standard makeTime and breakTime are not compliant to time.h, they add month and day onto the values.  With the correct TimeZone (TZ) used, using localTime against the RTC having UTC values, you'll get accurate DST calculations. 
        
        I haven't looked at your code, but the make and break Time functions outside of SmallRTC don't follow time.h for values, so the day and month will increase.
        */

        time_t epochTime = timeClient.getEpochTime();

        if(previousNTPTimeFull == 0) {
            previousNTPTimeFull = epochTime;
            debugLog("Setting initial ntp time for drift");
        } else {
            checkDrift();
            time_t currentTime = SRTC.MakeTime(*timeRTC);
            debugLog("Drift calc - NTP time:" + getFormattedTime(epochTime));
            debugLog("Drift calc - RTC time:" + getFormattedTime(currentTime));
            int ntpDriftCorrectionTmp = currentTime - epochTime;
            if(ntpDriftCorrection == 0 || ntpDriftCorrectionTmp > ACCEPTABLE_DRIFT) {
                previousNTPTimeDifference = epochTime - previousNTPTimeFull;
                ntpDriftCorrection = ntpDriftCorrectionTmp;
            } else {
                debugLog("Drift correction for delay: " + String(previousNTPTimeDifference) + " is fine");
            }
            debugLog("Drift calc - ntpDriftCorrection:" + String(ntpDriftCorrection));

            previousNTPTimeFull = epochTime;
            previousNTPCorrection = epochTime;
        }

        SRTC.BreakTime(epochTime, *timeRTC);
        saveRTC();
        timeClient.end();
        wakeUpManageRTC(); // After syncing time, remake the alarm
    } else {
        debugLog("NTP error");
    }
}

void checkDrift() {
    time_t currentTime = SRTC.MakeTime(*timeRTC);
    if(currentTime < 0) {
        debugLog("SOMETHING IS WRONG :(");
        readRTC();
        currentTime = SRTC.MakeTime(*timeRTC);
    }
    debugLog("currentTime: " + String(currentTime));
    debugLog("previousNTPCorrection: " + String(previousNTPCorrection));
    debugLog("previousNTPTimeDifference: " + String(previousNTPTimeDifference));
    if(previousNTPCorrection != 0 && currentTime - previousNTPCorrection >= previousNTPTimeDifference) {
        previousNTPCorrection = previousNTPCorrection + previousNTPTimeDifference;
        debugLog("Doing drift correction!");
        debugLog("ntpDriftCorrection: " + String(ntpDriftCorrection));
        debugLog("previousNTPCorrection: " + String(previousNTPCorrection));
        currentTime = currentTime + ntpDriftCorrection; // Not sure
        debugLog("currentTime: " + String(currentTime));
        SRTC.BreakTime(currentTime, *timeRTC);
        saveRTC();
    }
}
