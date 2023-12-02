#include "display.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));

void initDisplay(bool isFromWakeUp)
{
    log("initDisplay called:" + BOOL_STR(isFromWakeUp));
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

    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    // display.display(FULL_UPDATE);
}

#if DEBUG

void initDisplayDebug()
{
    log("Screen size: " + String(display.width()) + "x" + String(display.height()));
}

#endif
