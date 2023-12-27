#include "display.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));

void initDisplay(bool isFromWakeUp)
{
    debugLog("initDisplay called:" + BOOL_STR(isFromWakeUp));
    display.init(0, !isFromWakeUp, 10, true);
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
    // display.epd2._writeCommand(0x3C);
    // display.epd2._writeCommand(0x00);

    if (isFromWakeUp == false)
    {
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
        display.clearScreen();
    }
    display.setTextColor(GxEPD_BLACK);

    // Default values
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
}

RTC_DATA_ATTR int updateCounter = 0;
bool dUChange = false;
// Display update
void disUp(bool reallyUpdate)
{
    if (dUChange == true || reallyUpdate == true)
    {
        dUChange = false;
        if (updateCounter > FULL_DISPLAY_UPDATE_QUEUE)
        {
            updateCounter = 0;
            display.display(FULL_UPDATE);
        }
        else
        {
            updateCounter += 1;
            display.display(PARTIAL_UPDATE);
        }
    }
}

#if DEBUG

void initDisplayDebug()
{
    debugLog("Screen size: " + String(display.width()) + "x" + String(display.height()));
}

#endif
