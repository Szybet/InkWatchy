#include "display.h"

RTC_DATA_ATTR GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY));

void initDisplay()
{
    debugLog("initDisplay called");
#if EPD_CS != -1
    pinMode(EPD_CS, OUTPUT);
#endif
#if ATCHY_VER == YATCHY
    // Set screen cs to low
    gpioExpander.setPinMode(YATCHY_DISPLAY_CS, MCP_OUTPUT);
    // setPinPullUp(YATCHY_DISPLAY_CS, false); // Not needed, it's false at default
    gpioExpander.setPinState(YATCHY_DISPLAY_CS, LOW);
#endif
    pinMode(EPD_RESET, OUTPUT);
    pinMode(EPD_DC, OUTPUT);
    pinMode(EPD_BUSY, INPUT);
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    display.init(0, !bootStatus.fromWakeup, 10, true);
#elif ATCHY_VER == WATCHY_3
    SPI.begin(EPD_SPI_SCK, EPD_SPI_MISO, EPD_SPI_MOSI, EPD_SPI_SS);
    display.init(0, !bootStatus.fromWakeup, 10, true, SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
#elif ATCHY_VER == YATCHY
    SPI.begin(EPD_SPI_SCK, EPD_SPI_MISO, EPD_SPI_MOSI, EPD_SPI_SS);
    display.init(0, !bootStatus.fromWakeup, 10, true, SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
#endif
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

    display.setTextColor(GxEPD_BLACK);

    resetHoldManage();

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
        debugLog("Sleeping task because display had no update");
        delayTask(LOOP_NO_SCREEN_WRITE_DELAY_MS);
    }
}

#if DEBUG
void initDisplayDebug()
{
    debugLog("Screen size: " + String(display.width()) + "x" + String(display.height()));
}
#endif

bool resetReasonHold(esp_reset_reason_t resetReason)
{
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
    if (resetReason != ESP_RST_POWERON && resetReason != ESP_RST_SW && resetReason != ESP_RST_DEEPSLEEP)
    {
        return true;
    }
    else
    {
        return false;
    }
#elif ATCHY_VER == WATCHY_3
    // Because we are on a outdated version of esp idf ESP_RST_UNKNOWN is causes by jtag for example
    if (resetReason != ESP_RST_POWERON && resetReason != ESP_RST_SW && resetReason != ESP_RST_DEEPSLEEP && resetReason != ESP_RST_UNKNOWN)
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
    return true;
}

void resetHoldManage()
{
    if (bootStatus.fromWakeup == false && bootStatus.resetReason != ESP_RST_DEEPSLEEP)
    {
        display.fillScreen(GxEPD_WHITE);
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        uint16_t h = 100;
        centerText("Reset", &h);
        h = h + 11;
        setFont(getFont("dogicapixel4"));
        debugLog("Reset for reason: " + resetReasonToString(bootStatus.resetReason));
        centerText(resetReasonToString(bootStatus.resetReason), &h);
#if STOP_ON_RESET
        if (resetReasonHold(bootStatus.resetReason) == true) // ESP_RST_SW only for ESP.reset() in coredump cleaning
        {
            h = h + 30;
            centerText("Hold any button", &h);
            h = h + 10;
            centerText("To continue", &h);
        }
#endif
        display.display(FULL_UPDATE);
#if STOP_ON_RESET
        if (resetReasonHold(bootStatus.resetReason) == true) // ESP_RST_SW only for ESP.reset() in coredump cleaning
        {
            for (int i = 0; i < 10; i++)
            {
                Serial.println("Waiting for button input after reset...");
                delay(10);
            }
            flushLogs();
            turnOnButtons();
            while (useAllButtons() == None)
            {
                // gpioExpander.dumpAllRegisters();
                // debugLog("Battery voltage: " + String(BatteryRead()));
                delayTask(1000);
            }
            Serial.println("Exiting the stopper...");
        }
#endif
    }
}