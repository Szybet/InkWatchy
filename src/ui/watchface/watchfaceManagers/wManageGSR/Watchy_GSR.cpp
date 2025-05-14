#include "wManageGSR.h"
#include "rtcMem.h"

#if GSR_WATCHFACES
#include "Watchy_GSR.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> &WatchyGSR::display = *dis;

SmallRTC &WatchyGSR::SRTC = rM.SRTC;

uint16_t WatchyGSR::ForeColor() { return GxEPD_BLACK; }
uint16_t WatchyGSR::BackColor() { return GxEPD_WHITE; }

bool WatchyGSR::IsAM()
{
    return (timeRTCLocal.Hour < 12);
}

bool WatchyGSR::IsPM()
{
    return (timeRTCLocal.Hour >= 12);
}

wl_status_t WatchyGSR::WiFiStatus()
{
    return WiFi.status();
}

uint32_t WatchyGSR::CurrentStepCount()
{
    return getSteps();
}

float WatchyGSR::getBatteryVoltage()
{
    return rM.bat.curV;
}

int WatchyGSR::GetWeatherTemperature()
{
    OM_OneHourWeather wData = weatherGetDataHourly(WEATHER_WATCHFACE_HOUR_OFFSET);
    if (wData.fine == true)
    {
        return int(wData.temp);
    }
    return 0;
}

bool WatchyGSR::IsMetric()
{
    // Burger people are not here :)
    return true;
}

TimeData WatchTime;

RTC_DATA_ATTR Designing Design;

bool SafeToDraw()
{
    return true;
}

void initAddOn(WatchyGSR *gsr)
{
    return;
}

int AddWatchStyle(String name, WatchyGSR *gsr)
{
    return 1;
}

void WatchyGSR::InsertDrawWatchStyle(uint8_t StyleID)
{
    return;
}

bool WatchyGSR::BMAAvailable()
{
    return ACC_ENABLED;
}

void WatchyGSR::InsertDrawWeather(uint8_t StyleID, bool Status) {}

void WatchyGSR::drawWeather(bool Status)
{
    InsertDrawWeather(1, Status);
}

bool WatchyGSR::IsBatteryHidden()
{
    return true;
}

void WatchyGSR::WantWeather(bool Wanting) {}

String WatchyGSR::MakeMinutes(uint8_t Minutes)
{
    return (Minutes < 10 ? "0" : "") + String(Minutes);
}

bool WatchyGSR::IsWeatherAvailable() { return true; }

int WatchyGSR::GetWeatherID() { return 0; }

bool WatchyGSR::NoMenu() { return true; };

int WatchyGSR::GetWeatherTemperatureFeelsLike() { return GetWeatherTemperature(); }

void WatchyGSR::InsertInitWatchStyle(uint8_t StyleID) {}

#endif