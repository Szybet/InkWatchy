#include "display.h"

RTC_DATA_ATTR GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));

void initDisplay(bool isFromWakeUp)
{
    debugLog("initDisplay called: " + BOOL_STR(isFromWakeUp));
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
        debugLog("Clearing screen");
        display.fillScreen(GxEPD_WHITE);
        display.clearScreen();
    }
    display.setTextColor(GxEPD_BLACK);

    if (isFromWakeUp == false)
    {
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        uint16_t h = 100;
        centerText("Reset", &h);
        h = h + 11;
        setFont(getFont("dogicapixel4"));
        esp_reset_reason_t resetReason = esp_reset_reason();
        debugLog("Reset for reason: " + resetReasonToString(resetReason));
        centerText(resetReasonToString(resetReason), &h);
        display.display(FULL_UPDATE);
#if STOP_ON_RESET
        if (resetReason != ESP_RST_POWERON && resetReason != ESP_RST_SW) // ESP_RST_SW only for ESP.reset() in coredump cleaning
        {
            for (int i = 0; i < 10; i++)
            {
                Serial.println("Waiting for button input after reset...");
                delay(100);
            }
            Serial.flush();
            while (digitalRead(BACK_PIN) != HIGH && digitalRead(MENU_PIN) != HIGH && digitalRead(UP_PIN) != HIGH && digitalRead(DOWN_PIN) != HIGH)
            {
                delay(1000);
            }
            Serial.println("Exiting the stopper...");
        }
#endif
    }
    // Default values
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
}

RTC_DATA_ATTR int updateCounter = 0;
bool dUChange = false;
// Display update
void disUp(bool reallyUpdate, bool ignoreCounter, bool ignoreSleep)
{
    // debugLog("dis up executed");
    bool updatedScreen = false;
    if (dUChange == true || reallyUpdate == true)
    {
        dUChange = false;
        if (updateCounter >= FULL_DISPLAY_UPDATE_QUEUE && ignoreCounter == false)
        {
            updateCounter = 0;
            display.display(FULL_UPDATE);
            updatedScreen = true;
        }
        else
        {
            if (ignoreCounter == false)
            {
                updateCounter += 1;
            }
            display.display(PARTIAL_UPDATE);
            updatedScreen = true;
        }
#if SCOM_TASK_ENABLED
        scomChanged = true;
#endif
    }
    if (ignoreSleep == false && updatedScreen == false)
    {
        // debugLog("Sleeping task because display had no update");
#if DEBUG == false
        delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS);
#endif
    }
}

#if DEBUG

void initDisplayDebug()
{
    debugLog("Screen size: " + String(display.width()) + "x" + String(display.height()));
}

#endif
