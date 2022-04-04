#ifndef WATCHY_GSR_H
#define WATCHY_GSR_H

#include <core_version.h>
#include "Defines_GSR.h"
#include "Web-HTML.h"
#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <SmallRTC.h>
#include <SmallNTP.h>
#include <Olson2POSIX.h>
#include "GxEPD2_BW.h"
#include <mbedtls/base64.h>
#include <Wire.h>
#include <StableBMA.h>

#include "Icons_GSR.h"
#include "ArduinoNvs.h"

#include "Bronova_Regular13pt7b.h"
#include "aAntiCorona10pt7b.h"
#include "aAntiCorona11pt7b.h"
#include "aAntiCorona12pt7b.h"
#include "aAntiCorona13pt7b.h"
#include "aAntiCorona14pt7b.h"
#include "aAntiCorona15pt7b.h"
#include "aAntiCorona16pt7b.h"
#include "aAntiCorona36pt7b.h"

class WatchyGSR{
    public:
        static SmallRTC SRTC;
        static SmallNTP SNTP;
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
        static constexpr const char* Build = "1.4.3E";
        enum DesOps {dSTATIC, dLEFT, dRIGHT, dCENTER};
    public:
        WatchyGSR();
        virtual void init(String datetime = "") final;
        void showWatchFace();
        void drawWatchFace(); //override this method for different watch faces
        void drawTime();
        void drawDay();
        void drawDate();
        void drawYear();
        virtual void handleButtonPress(uint8_t Pressed) final;
        virtual void deepSleep() final;
        virtual float getBatteryVoltage() final;
        virtual float BatteryRead() final;
        virtual bool IsDark() final;
        IRAM_ATTR virtual void handleInterrupt() final;
        void drawChargeMe(bool Dark = false);
        void drawStatus();
        virtual void VibeTo(bool Mode) final;
        virtual String MakeTime(int Hour, int Minutes, bool& Alarm) final;
        virtual String MakeHour(uint8_t Hour) final;
        virtual String MakeMinutes(uint8_t Minutes) final;
        virtual uint16_t ForeColor() final;
        virtual uint16_t BackColor() final;
        void InsertPost();
        bool OverrideBitmap();
        bool OverrideSleepBitmap();
        void InsertDefaults();
        void InsertOnMinute();
        void InsertWiFi();
        void InsertWiFiEnding();
        void InsertAddWatchStyles();
        void InsertDrawWatchStyle(uint8_t StyleID);
        void InsertInitWatchStyle(uint8_t StyleID);
        virtual uint8_t AddWatchStyle(String StyleName) final;
        String InsertNTPServer();
        virtual void AllowDefaultWatchStyles(bool Allow = true) final;
        virtual void AskForWiFi() final;
        virtual wl_status_t currentWiFi() final;
        virtual void endWiFi() final;
        virtual void getAngle(uint16_t Angle, uint8_t Away, uint8_t &X, uint8_t &Y) final;
        virtual bool SafeToDraw() final;
        void initWatchFaceStyle();
        void drawWatchFaceStyle();
   private:
        void setStatus(String Status);
        void drawMenu();
        void setFontFor(String O, const GFXfont *Normal, const GFXfont *Small, const GFXfont *Smaller, byte Gutter = 5);
        void drawData(String dData, byte Left, byte Bottom, WatchyGSR::DesOps Style, byte Gutter, bool isTime = false, bool PM = false);
        void GoDark();
        void detectBattery();
        void ProcessNTP();
        void UpdateUTC();
        void UpdateClock();
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
        void CheckCD();
        void CalculateTones();
        void StopCD();
        uint8_t getToneTimes(uint8_t ToneIndex);
        String getReduce(uint8_t Amount);
        void monitorSteps();
        uint8_t getButtonPins();
        uint8_t getButtonMaskToID(uint64_t HW);
        uint8_t getSwapped(uint8_t pIn);
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
        bool Showing();
        void RefreshCPU();
        void RefreshCPU(int Value);
        uint8_t getTXOffset(wifi_power_t Current);
        void DisplayInit(bool ForceDark = false);
        void DisplaySleep();
};
extern RTC_DATA_ATTR StableBMA SBMA;
#endif
