#include "wManageGSR.h"
#include "rtcMem.h"

#if GSR_WATCHFACES
#include "Watchy_GSR.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>& WatchyGSR::display = *dis;

SmallRTC& WatchyGSR::SRTC = rM.SRTC;

uint16_t WatchyGSR::ForeColor() { return GxEPD_BLACK; }
uint16_t WatchyGSR::BackColor() { return GxEPD_WHITE; }

bool WatchyGSR::IsAM() {
    return (timeRTCLocal.Hour < 12);
}

bool WatchyGSR::IsPM() {
    return (timeRTCLocal.Hour >= 12);
}

wl_status_t WatchyGSR::WiFiStatus() {
    return WiFi.status();
}

uint32_t WatchyGSR::CurrentStepCount() {
    return getSteps();
}

float WatchyGSR::getBatteryVoltage() {
    return rM.bat.curV;
}

int WatchyGSR::GetWeatherTemperature() {
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        return int(wData.temp);
    }
    return 0;
}

bool WatchyGSR::IsMetric() {
    // Burger people are not here :)
    return true;
}

TimeData WatchTime;

bool SafeToDraw() {
    return true;
}

void initAddOn(WatchyGSR* gsr) {
    // Nothing lol
    return;
}

int AddWatchStyle(String name, WatchyGSR* gsr) {
    // Nothing too
    return 0;
}

void WatchyGSR::InsertDrawWatchStyle(uint8_t StyleID) {
    return;
}

#endif