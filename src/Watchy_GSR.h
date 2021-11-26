#ifndef WATCHY_GSR_H
#define WATCHY_GSR_H

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
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#ifndef PCF8563RTC
#include <DS3232RTC.h>
#else
#include <PCF8563.h>
#endif
#include "GxEPD2_BW.h"
#include "mbedtls/base64.h"
#include <Wire.h>
#include <bma.h>

#include "icons.h"
#include "Olsen2POSIX.h"
#include "ArduinoNvs.h"

#include "aAntiCorona15pt7b.h"
#include "Bronova_Regular13pt7b.h"
#include "aAntiCorona16pt7b.h"
#include "aAntiCorona36pt7b.h"
#include "aAntiCorona12pt7b.h"

class WatchyGSR{
    public:
#ifndef PCF8563RTC
        static DS3232RTC RTC;
#else
        static PCF8563 RTC;
#endif
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    public:
        WatchyGSR();
        void init();
        void showWatchFace();
        void drawWatchFace(); //override this method for different watch faces
        void drawTime();
        void drawDay();
        void drawDate();
        void drawYear();
        void handleButtonPress(uint8_t Pressed);
        virtual void deepSleep();
        float getBatteryVoltage();
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
        static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
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
        String RetrieveSettings();
        void RecordSettings();
        void SetTurbo();
        bool InTurbo();
        bool DarkWait();
        bool Showing();
        void RefreshCPU();
        void RefreshCPU(int Value);
        void DBug(String Value);
        String ToHex(uint64_t Value);
};

extern RTC_DATA_ATTR BMA423 sensor;
extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
extern RTC_DATA_ATTR bool BLE_CONFIGURED;

#endif
