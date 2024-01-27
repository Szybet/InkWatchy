#include "ntp.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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

        SRTC.BreakTime(epochTime, *timeRTC);
        saveRTC();
        timeClient.end();
        wakeUpManageRTC(); // After syncing time, remake the alarm
    } else {
        debugLog("NTP error");
    }
}
