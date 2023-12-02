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
    log("Get RTC battery level: " + String(SRTC.getRTCBattery(false)));
    log("Get critical RTC battery level: " + String(SRTC.getRTCBattery(true)));
}

void loopRTCDebug()
{
    log("SRTC.isOperating: " + BOOL_STR(SRTC.isOperating()));
    log("SRTC.getADCPin: " + String(SRTC.getADCPin()));
    log("SRTC.temperature(): " + String(SRTC.temperature()));
}

void dumpRTCTime()
{
    readRTC();
    log("Second: " + String(timeRTC.Second));
    log("Minute: " + String(timeRTC.Minute));
    log("Hour: " + String(timeRTC.Hour));
    log("Day: " + String(timeRTC.Day));
    log("Month: " + String(timeRTC.Month));
    log("Year: " + String(timeRTC.Year));
}
#endif
