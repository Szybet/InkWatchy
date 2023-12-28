#include "RTC.h"

tmElements_t timeRTC;

RTC_DATA_ATTR SmallRTC SRTC;
RTC_DATA_ATTR int rtcDelayMs;

void initRTC()
{
    SRTC.init();
    HWVer = SRTC.getWatchyHWVer();
    rtcDelayMs = millis();
    readRTC(true);
}

void saveRTC()
{

    SRTC.set(timeRTC);
}

void readRTC(bool force)
{
    if (millis() - rtcDelayMs > RTC_READ_DELAY_MS || force == true)
    {
        debugLog("Reading RTC");
        SRTC.read(timeRTC);
        rtcDelayMs = millis();
    }
}

String getHourMinute()
{
    String h = String(timeRTC.Hour);
    if (h.length() == 1)
    {
        h = "0" + h;
    }

    String m = String(timeRTC.Minute);
    if (m.length() == 1)
    {
        m = "0" + m;
    }

    return h + ":" + m;
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
