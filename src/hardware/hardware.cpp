#include "hardware.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));
RTC_DATA_ATTR SmallRTC SRTC;

RTC_DATA_ATTR float HWVer;
int UP_PIN = 32;
uint64_t UP_MASK = GPIO_SEL_32;

void setupHardware()
{
    isDebug(Serial.begin(230400));
    log("Watchy is starting!");

    display.init(0, true, 10, true);
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    /*
    Here, to remove border you need to go to GxEPD2_154_D67::_InitDisplay()
    .pio/libdeps/watchy/GxEPD2/src/epd/GxEPD2_154_D67.cpp
    and change the line under this:
    _writeCommand(0x3C); // BorderWavefrom
    to this:
    _writeData(0x00);
    */
    // And we can do this from here! ( by using the magic script )
    //display.epd2._writeCommand(0x3C);
    //display.epd2._writeCommand(0x00);

    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    //display.display(FULL_UPDATE);

    SRTC.init();

    HWVer = SRTC.getWatchyHWVer();
    if (SRTC.getType() == PCF8563)
    {
        if (HWVer == 1.5)
        {
            UP_PIN = 32;
            UP_MASK = GPIO_SEL_32;
        }
        else
        {
            UP_PIN = 35;
            UP_MASK = GPIO_SEL_35;
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
        log("Menu button pressed");
    }
    else if (digitalRead(BACK_PIN) == 1)
    {
        log("Back button pressed");
    }
    else if (digitalRead(UP_PIN) == 1)
    {
        log("Up button pressed");
    }
    else if (digitalRead(DOWN_PIN) == 1)
    {
        log("Down button pressed");
    }
}

void showSetupResults()
{
    log("Get RTC battery level: " + String(SRTC.getRTCBattery(false)));
    log("Get critical RTC battery level: " + String(SRTC.getRTCBattery(true)));
    log("Up button pin number: " + String(UP_PIN));
    log("Hardware version: " + String(HWVer));
    log("Screen size: " + String(display.width()) + "x" + String(display.height()));
    log("SRTC.isOperating: " + BOOL_STR(SRTC.isOperating()));
    log("SRTC.getADCPin: " + String(SRTC.getADCPin()));
}
#endif
