#ifndef WATCHY_GSR_H
#define WATCHY_GSR_H

#include "config.h"
#include "Defines_GSR.h"
#include "Web-HTML.h"
#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>;
#include <Update.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <DS3232RTC.h>
#include "GxEPD2_BW.h"
#include "mbedtls/base64.h"
#include <Wire.h>
#include "BLE.h"
#include "bma.h"

#include "icons.h"
#include "Olsen2POSIX.h"

//#include "esp_system.h"
//#include "nvs_flash.h"
//#include "nvs.h"
//#include "nvs_handle.hpp"

#include "aAntiCorona15pt7b.h";
#include "Bronova_Regular13pt7b.h"
#include "aAntiCorona16pt7b.h"
#include "aAntiCorona36pt7b.h"
#include "aAntiCorona12pt7b.h"

class Watchy{
    public:
        static DS3232RTC RTC;
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    public:
        Watchy();
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
        void detectBattery();
        void ProcessNTP();
        void UpdateUTC();
        void UpdateClock();
        void _rtcConfig();
        void _bmaConfig();
        static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        String MakeTime(int Hour, int Minutes, bool& Alarm);
        String MakeHour(uint8_t Hour);
        String MakeMinutes(uint8_t Minutes);
        String MakeSteps(uint32_t uSteps);
        void CheckAlarm(int I);
        void CheckCD();
        void CalculateTones();
        void StopCD();
        uint8_t getToneTimes(uint8_t ToneIndex);
        void monitorSteps();
//        String GetStoredTimezone();
//        virtual bool StoreTimezone(String Timezone);
        uint8_t getButtonPins();
        uint8_t getButtonMaskToID(uint64_t HW);
        uint8_t getSwapped(uint8_t pIn);
        void ScreenRefresh();
        void setupDefaults();
        void initZeros();
        String GetSettings();
        void StoreSettings(String FromUser);
        void SetTurbo();
        bool InTurbo();
        void DBug(String Value);
        String ToHex(uint64_t Value);
};

extern RTC_DATA_ATTR BMA423 sensor;
extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
extern RTC_DATA_ATTR bool BLE_CONFIGURED;

#endif
