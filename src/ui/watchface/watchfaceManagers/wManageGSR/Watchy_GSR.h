#pragma once

#include "minimalDefines.h"

#if GSR_WATCHFACES

#define GSR_AutoFore 0xC0C0

class WatchyGSR
{
public:
    static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>& display;
    static SmallRTC& SRTC;
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
    virtual bool BMAAvailable() final;
    virtual void drawWeather(bool Status = false);
    virtual void InsertDrawWeather(uint8_t StyleID, bool Status = false);
    virtual bool IsBatteryHidden() final;
    enum DesOps {dSTATIC, dLEFT, dRIGHT, dCENTER};
    virtual void WantWeather(bool Wanting);
    virtual String MakeMinutes(uint8_t Minutes) final;
    virtual bool IsWeatherAvailable() final;
    virtual int GetWeatherID() final;
    virtual bool NoMenu() final;
    virtual int GetWeatherTemperatureFeelsLike() final;
    virtual void InsertInitWatchStyle(uint8_t StyleID);
    virtual void RegisterWatchFaces();
};

struct TimeData final {
    tmElements_t Local;
};

extern TimeData WatchTime;

struct MenuPOS {
    byte Gutter; // 3
    byte Top;    // MenuTop 72
    byte Header; // HeaderY 97
    byte Data;   // DataY 138
    const GFXfont *Font; // Menu Font.
    const GFXfont *FontSmall; // Menu Font.
    const GFXfont *FontSmaller; // Menu Font.
};

struct FacePOS {
    const unsigned char *Bitmap;  // Null
    const unsigned char *SleepBitmap;  // Null
    byte Gutter; // 4
    byte Time;   // TimeY 56
    byte TimeHeight; // 45
    uint16_t TimeColor;  // Font Color.
    const GFXfont *TimeFont; // Font.
    WatchyGSR::DesOps TimeStyle; // dCENTER
    byte TimeLeft;  // Only for dSTATIC
    byte Day;    // DayY 101
    byte DayGutter; // 4
    uint16_t DayColor;  // Font Color.
    const GFXfont *DayFont; // Font.
    const GFXfont *DayFontSmall; // Font.
    const GFXfont *DayFontSmaller; // Font.
    WatchyGSR::DesOps DayStyle; // dCENTER
    byte DayLeft;  // Only for dSTATIC
    byte Date;   // DateY 143
    byte DateGutter; // 4
    uint16_t DateColor;  // Font Color.
    const GFXfont *DateFont; // Font.
    const GFXfont *DateFontSmall; // Font.
    const GFXfont *DateFontSmaller; // Font.
    WatchyGSR::DesOps DateStyle; // dCENTER
    byte DateLeft;  // Only for dSTATIC
    byte Year;   // YearY 186
    uint16_t YearColor;  // Font Color.
    const GFXfont *YearFont; // Font.
    WatchyGSR::DesOps YearStyle; // dCENTER
    byte YearLeft;  // Only for dSTATIC
};

struct StatusPOS {
    byte WIFIx;  // NTPX 5
    byte WIFIy;  // NTPY 193
    byte BATTx;  // 155
    byte BATTy;  // 178
    bool Inverted;  // Opposite Colors.
    bool BatteryInverted; // Just for the battery indicator.
};

struct Designing final {
    struct MenuPOS Menu;
    struct FacePOS Face;
    struct StatusPOS Status;
};

extern Designing Design;

bool SafeToDraw();

void initAddOn(WatchyGSR* gsr);

int AddWatchStyle(String name, WatchyGSR* gsr);

#endif