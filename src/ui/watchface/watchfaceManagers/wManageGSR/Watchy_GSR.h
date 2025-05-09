#pragma once

#include "minimalDefines.h"

#if GSR_WATCHFACES

class WatchyGSR
{
public:
    static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    static SmallRTC SRTC;
    virtual uint16_t ForeColor() final;
    virtual uint16_t BackColor() final;
    virtual bool IsAM() final;
    virtual bool IsPM() final;
    virtual wl_status_t WiFiStatus() final;
    virtual uint32_t CurrentStepCount() final;
    static float getBatteryVoltage();
    virtual int GetWeatherTemperature() final;
    virtual bool IsMetric() final;
    virtual void InsertDrawWatchStyle(uint8_t StyleID); // This is overwritten
};

struct TimeData final {
    tmElements_t Local;
};

extern TimeData WatchTime;

bool SafeToDraw();

void initAddOn(WatchyGSR* gsr);

int AddWatchStyle(String name, WatchyGSR* gsr);

#endif