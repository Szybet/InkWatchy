#include "RTC.h"

tmElements_t timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;

void initRTC()
{
    SRTC.init();
    HWVer = SRTC.getWatchyHWVer();
}

void saveRTC()
{
    SRTC.set(timeRTC);
}

void readRTC()
{
    SRTC.read(timeRTC);
}

#if DEBUG
void initRTCDebug()
{
    debugLog("Get RTC battery level: " + String(SRTC.getRTCBattery(false)));
    debugLog("Get critical RTC battery level: " + String(SRTC.getRTCBattery(true)));
}

void loopRTCDebug()
{
    debugLog("SRTC.isOperating: " + BOOL_STR(SRTC.isOperating()));
    debugLog("SRTC.getADCPin: " + String(SRTC.getADCPin()));
    debugLog("SRTC.temperature(): " + String(SRTC.temperature()));
}

void dumpRTCTime()
{
    readRTC();
    debugLog("Second: " + String(timeRTC.Second));
    debugLog("Minute: " + String(timeRTC.Minute));
    debugLog("Hour: " + String(timeRTC.Hour));
    debugLog("Day: " + String(timeRTC.Day));
    debugLog("Month: " + String(timeRTC.Month));
    debugLog("Year: " + String(timeRTC.Year));
}
#endif
