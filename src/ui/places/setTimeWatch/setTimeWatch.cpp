#include "setTimeWatch.h"

#if SET_TIME_GUI

static bool setTimeWatchInitialized = false;

// Helper to add seconds to hour/minute (no day wrap, for simplicity)
void addSecondsToHourMinute(uint8_t baseHour, uint8_t baseMinute, int32_t seconds, int &outHour, int &outMinute)
{
    int totalMinutes = baseHour * 60 + baseMinute + seconds / 60;
    // Wrap around 24 hours
    if (totalMinutes < 0)
        totalMinutes += 24 * 60;
    if (totalMinutes >= 24 * 60)
        totalMinutes -= 24 * 60;
    outHour = (totalMinutes / 60) % 24;
    outMinute = totalMinutes % 60;
}

void initSetTimeWatch()
{
    if (!setTimeWatchInitialized)
    {
        // Correct: subtract timezone offset (if offset is negative for GMT+1)
        int localHour, localMinute;
        addSecondsToHourMinute(
            timeRTCUTC0.Hour,
            timeRTCUTC0.Minute,
            -(int32_t)(timeZoneOffset), // <-- FIXED SIGN
            localHour,
            localMinute);
        setTimeHour = localHour;
        setTimeMinute = localMinute;
        initSetTime();
        setTimeWatchInitialized = true;
    }
}

void loopSetTimeWatch()
{
    loopSetTime();
}

void exitSetTimeWatch()
{
    setTimeWatchInitialized = false;
    exitSetTime(); // Commit UI edits

    // Correct: add timezone offset to get back to UTC
    int utcHour, utcMinute;
    addSecondsToHourMinute(
        setTimeHour,
        setTimeMinute,
        (int32_t)(timeZoneOffset), // <-- FIXED SIGN
        utcHour,
        utcMinute);

    tmElements_t newTime = timeRTCUTC0;
    newTime.Hour = utcHour;
    newTime.Minute = utcMinute;
    newTime.Second = 0;
    saveRTC(newTime);
    readRTC();
}
#endif