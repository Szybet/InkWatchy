#ifndef WATCHY_GSR_H
#define WATCHY_GSR_H

#include <core_version.h>
#include <Watchy.h>
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
#include <bma.h>

#include "icons.h"
#include "ArduinoNvs.h"

#include "aAntiCorona15pt7b.h"
#include "Bronova_Regular13pt7b.h"
#include "aAntiCorona16pt7b.h"
#include "aAntiCorona36pt7b.h"
#include "aAntiCorona12pt7b.h"

class WatchyGSR{
    public:
        static SmallRTC SRTC;
//        static WatchyRTC SRTC;
        static SmallNTP SNTP;
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
        static constexpr const char* Build = "1.3.6";
    public:
        WatchyGSR();
        void init(String datetime = "");
        void showWatchFace();
        void drawWatchFace(); //override this method for different watch faces
        void drawTime();
        void drawDay();
        void drawDate();
        void drawYear();
        void handleButtonPress(uint8_t Pressed);
        virtual void deepSleep();
        float getBatteryVoltage();
        float BatteryRead();
        bool IsDark();
        IRAM_ATTR void handleInterrupt();
   private:
        void drawChargeMe();
        void drawStatus();
        void setStatus(String Status);
        void drawMenu();
        void VibeTo(bool Mode);
        //void handleAccelerometer();
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
        uint16_t FontColor();
        String MakeTime(int Hour, int Minutes, bool& Alarm);
        String MakeHour(uint8_t Hour);
        String MakeSeconds(uint8_t Seconds);
        String MakeTOD(uint8_t Hour, bool AddZeros);
        String MakeMinutes(uint8_t Minutes);
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
        void AskForWiFi();
        void processWiFiRequest();
        String WiFiIndicator(uint8_t Index);
        void UpdateWiFiPower(String SSID, String PSK);
        void UpdateWiFiPower(String SSID);
        void UpdateWiFiPower(uint8_t PWRIndex = 0);
        wl_status_t currentWiFi();
        void endWiFi();
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

extern RTC_DATA_ATTR BMA423 sensor;
#endif
