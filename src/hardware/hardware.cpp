#include "hardware.h"

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
    /*
    Here, to remove border you need to go to GxEPD2_154_D67::_InitDisplay()
    and change the line under this:
    _writeCommand(0x3C); // BorderWavefrom
    to this:
    _writeData(0x00);
    */
    // And we can do this from here! ( by using the magic script )
    //display.epd2._writeCommand(0x3C);
    //display.epd2._writeCommand(0x00);

    display.setFullWindow();
    display.display(FULL_UPDATE);

    SRTC.init();

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

void showSetupResults()
{
    Serial.print("Get RTC battery level: ");
    Serial.println(SRTC.getRTCBattery(false));
    Serial.print("Get critical RTC battery level: ");
    Serial.println(SRTC.getRTCBattery(true));
    Serial.print("Up button pin number: ");
    Serial.println(UP_PIN);
    Serial.print("Hardware version: ");
    Serial.println(HWVer);
}

#endif
