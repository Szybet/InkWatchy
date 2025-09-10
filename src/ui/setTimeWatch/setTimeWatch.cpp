#include "setTimeWatch.h"
#include "rtcMem.h"
#include "../../reUse/setTime/setTime.h"

static bool setTimeWatch_initialized = false;

void initSetTimeWatch() {
    if (!setTimeWatch_initialized) {
        setTimeHour = timeRTCUTC0.Hour;
        setTimeMinute = timeRTCUTC0.Minute;
        initSetTime();
        setTimeWatch_initialized = true;
    }
}

void loopSetTimeWatch() {
    loopSetTime();
}

void exitSetTimeWatch() {
    setTimeWatch_initialized = false;
    exitSetTime(); // <-- Commit UI values to globals!
    tmElements_t newTime = timeRTCUTC0;
    // Use the values selected by the user:
    newTime.Hour = setTimeHour;
    newTime.Minute = setTimeMinute;
    newTime.Second = 0;
    saveRTC(newTime);
    readRTC();
}
