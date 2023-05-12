#ifndef WATCHY_GSR_H
#define WATCHY_GSR_H

#include <core_version.h>
#include "Defines_GSR.h"
#include "Web-HTML.h"
#include <Arduino.h>
#include <esp_partition.h>
#include <FunctionalInterrupt.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <SmallRTC.h>
#include <SmallNTP.h>
#include <Olson2POSIX.h>
#include <GxEPD2_BW.h>
#include "Locale_GSR.h"
#include <mbedtls/base64.h>
#include <Wire.h>
#include <StableBMA.h>
#include "Fonts_GSR.h"
#include "Icons_GSR.h"
#include "ArduinoNvs.h"
#include <esp32-hal.h>

class WatchyGSR{
    public:
        static SmallRTC SRTC;
        static SmallNTP SNTP;
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
        static constexpr const char* Build = "1.4.6A";
        enum DesOps {dSTATIC, dLEFT, dRIGHT, dCENTER};

    public:
        WatchyGSR();
        virtual void init(String datetime = "") final;
        virtual void StartWeb() final;
        virtual void showWatchFace();
        virtual void drawWatchFace(); //override this method for different watch faces
        virtual void drawTime(uint8_t Flags = 0); // 32 means no AM/PM, 64 means add padding to <10 hour.
        virtual void drawDay();
        virtual void drawDate(bool Short = false);  // Short month can be used.
        virtual void drawYear();
        virtual void handleButtonPress(uint8_t Pressed) final;
        virtual void deepSleep() final;
        virtual float getBatteryVoltage() final;
        virtual float BatteryRead() final;
        virtual bool IsDark() final;
        virtual bool IsAM() final;
        virtual bool IsPM() final;
        virtual String GetLangWebID() final;
        IRAM_ATTR virtual void handleInterrupt() final;
        void drawChargeMe(bool Dark = false);
        void drawStatus();
        static void VibeTo(bool Mode);
        virtual String MakeTime(int Hour, int Minutes, bool& Alarm) final; // For Hour | 32 means no AM/PM, | 64 means add padding to <10 hour.
        virtual String MakeHour(uint8_t Hour) final;
        virtual String MakeMinutes(uint8_t Minutes) final;
        virtual void ClockSeconds() final;
        virtual uint16_t ForeColor() final;
        virtual uint16_t BackColor() final;
        virtual void InsertPost();
        virtual bool OverrideBitmap();
        virtual bool OverrideSleepBitmap();
        virtual void InsertDefaults();
        virtual void InsertOnMinute();
        virtual void InsertWiFi();
        virtual void InsertWiFiEnding();
        virtual void InsertAddWatchStyles();
        virtual void InsertDrawWatchStyle(uint8_t StyleID);
        virtual void InsertInitWatchStyle(uint8_t StyleID);
        virtual bool InsertNeedAwake(bool GoingAsleep);
        virtual bool InsertHandlePressed(uint8_t SwitchNumber, bool &Haptic, bool &Refresh);
        virtual void OverrideDefaultMenu(bool Override);
        virtual void ShowDefaultMenu() final;
        virtual uint8_t AddWatchStyle(String StyleName) final;
        virtual String InsertNTPServer();
        virtual void AllowDefaultWatchStyles(bool Allow = true) final;
        virtual void AskForWiFi() final;
        virtual wl_status_t currentWiFi() final;
        virtual void endWiFi() final;
        virtual void getAngle(uint16_t Angle, uint8_t Width, uint8_t Height, uint8_t &X, uint8_t &Y) final;
        virtual bool SafeToDraw() final;
        virtual bool NoMenu() final;
        virtual void initWatchFaceStyle();
        virtual void drawWatchFaceStyle();
   private:
        void setStatus(String Status);
        void drawMenu();
        void setFontFor(String O, const GFXfont *Normal, const GFXfont *Small, const GFXfont *Smaller, byte Gutter = 5);
        void drawData(String dData, byte Left, byte Bottom, WatchyGSR::DesOps Style, byte Gutter, bool isTime = false, bool PM = false);
        void GoDark();
        void detectBattery();
        void ProcessNTP();
        void UpdateUTC(bool OnlyRead = false);
        void UpdateClock();
        void UpdateTimerDown();
        bool TimerAbuse();
        static void SoundAlarms(void * parameter);
        void ManageTime();
        void _rtcConfig();
        void _bmaConfig();
        void UpdateBMA();
        static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        void UpdateFonts();
        String MakeTOD(uint8_t Hour, bool AddZeros);
        String MakeSeconds(uint8_t Seconds);
        String MakeSteps(uint32_t uSteps);
        void CheckAlarm(int I);
        void CalculateTones();
        void StartCD();
        void StopCD();
        static uint8_t getToneTimes(uint8_t ToneIndex);
        String getReduce(uint8_t Amount);
        void monitorSteps();
        uint8_t getButtonPins();
        uint8_t getButtonMaskToID(uint64_t HW);
        uint8_t getSWValue(bool SW1, bool SW2, bool SW3, bool SW4);
        void processWiFiRequest();
        String WiFiIndicator(uint8_t Index);
        void UpdateWiFiPower(String SSID, String PSK);
        void UpdateWiFiPower(String SSID);
        void UpdateWiFiPower(uint8_t PWRIndex = 0);
        static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
        String buildWiFiAPPage();
        void parseWiFiPageArg(String ARG, String DATA);
        void setupDefaults();
        String APIDtoString(uint8_t Index);
        String PASStoString(uint8_t Index);
        void initZeros();
        String GetSettings();
        void StoreSettings(String FromUser);
        void RetrieveSettings();
        void RecordSettings();
        bool OkNVS(String FaceName);
        void SetNVS(String FaceName, bool Enabled = true);
        void NVSEmpty();
        void SetTurbo();
        bool InTurbo();
        bool BedTime();
        bool UpRight();
        bool DarkWait();
        static bool Showing();
        void RefreshCPU();
        void RefreshCPU(int Value);
        bool OTA();
        uint8_t getTXOffset(wifi_power_t Current);
        void DisplayInit(bool ForceDark = false);
        void DisplaySleep();
};

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
};
struct Designing final {
    struct MenuPOS Menu;
    struct FacePOS Face;
    struct StatusPOS Status;
};

struct TimeData final {
    time_t UTC_RAW;           // Copy of the UTC on init.
    tmElements_t UTC;         // Copy of UTC only split up for usage.
    tmElements_t Local;       // Copy of the Local time on init.
    String TimeZone;          // The location timezone, not the actual POSIX.
    unsigned long EPSMS;      // Milliseconds (rounded to the enxt minute) when the clock was updated via NTP.
    bool NewMinute;           // Set to True when New Minute happens.
    time_t TravelTest;        // For Travel Testing.
    int32_t Drifting;         // The amount to add to UTC_RAW after reading from the RTC.
    int64_t WatchyRTC;        // Counts Microseconds from boot.
    bool DeadRTC;             // Set when Drift fails to get a good count less than 30 seconds.
    uint8_t NextAlarm;        // Next index that will need to wake the Watchy from sleep to fire.
    bool BedTime;             // If the hour is within the Bed Time settings.
};

extern Designing Design;
extern TimeData WatchTime;
extern StableBMA SBMA;
extern LocaleGSR LGSR;
#endif
