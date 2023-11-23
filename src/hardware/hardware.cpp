#include "hardware.h"

#include "defines/defines.h"
#include <GxEPD2_BW.h>
#include <SmallRTC.h>

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));
RTC_DATA_ATTR SmallRTC SRTC;

RTC_DATA_ATTR float HWVer;
int UP_PIN = 32;

void setupHardware()
{
#if DEBUG
    Serial.begin(230400);
    Serial.println("Watchy is starting!");
#endif

    display.init(0, true, 10, true);
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    display.setFullWindow();
    display.display(FULL_UPDATE);

    SRTC.init();
    SRTC.getRTCBattery();
    SRTC.getRTCBattery(true);

    HWVer = SRTC.getWatchyHWVer();
    if (SRTC.getType() == PCF8563)
    {
        if (HWVer == 1.5)
        {
            UP_PIN = 32;
        }
        else
        {
            UP_PIN = 35;
        }
    }

    pinMode(MENU_PIN, INPUT);
    pinMode(BACK_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(DOWN_PIN, INPUT);
}

#if DEBUG
void dumpButtons()
{
    if (digitalRead(MENU_PIN) == 1)
    {
        Serial.println("Menu button pressed");
    }
    else if (digitalRead(BACK_PIN) == 1)
    {
        Serial.println("Back button pressed");
    }
    else if (digitalRead(UP_PIN) == 1)
    {
        Serial.println("Up button pressed");
    }
    else if (digitalRead(DOWN_PIN) == 1)
    {
        Serial.println("Down button pressed");
    }
}

#endif