#include "display.h"
#include "rtcMem.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> *dis = &rM.display;

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
    dis->epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    dis->init(0, !bootStatus.fromWakeup, 10, true);
#elif ATCHY_VER == WATCHY_3
    SPI.begin(EPD_SPI_SCK, EPD_SPI_MISO, EPD_SPI_MOSI, EPD_SPI_SS);
    dis->init(0, !bootStatus.fromWakeup, 10, true, SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
#elif ATCHY_VER == YATCHY
    SPI.begin(EPD_SPI_SCK, EPD_SPI_MISO, EPD_SPI_MOSI, EPD_SPI_SS);
    dis->init(0, !bootStatus.fromWakeup, 10, true, SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
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
    // dis->epd2._writeCommand(0x3C);
    // dis->epd2._writeCommand(0x00);

    dis->setTextColor(GxEPD_BLACK);

    // Only on first boot, only to be extra sure
#if SCREEN_PARTIAL_GREY_WORKAROUND || LP_CORE_TEST_ENABLED
    if (bootStatus.fromWakeup == false)
    {
        dis->setPartialWindow(0, 0, 200, 200);
        dis->clearScreen();
        dis->fillScreen(GxEPD_WHITE);
        updateDisplay(FULL_UPDATE);
    }
#endif
#if LP_CORE_TEST_ENABLED == false
    resetHoldManage();
#endif

    // Default values
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
}

bool dUChange = false;
// Display update
void disUp(bool reallyUpdate, bool ignoreCounter, bool ignoreSleep)
{
    // debugLog("dis up executed");
    bool updatedScreen = false;
    if (dUChange == true || reallyUpdate == true)
    {
        dUChange = false;
#if LP_CORE == true
        if (bootStatus.reason == ulp || (screenForceNextFullTimeWrite == true && currentPlaceIndex == 0))
        {
            screenForceNextFullTimeWrite = false;
            lpCoreScreenPrepare(false, false);
            showTimeFullGlobal();
            clearLpCoreRtcMem();
        }
#endif
        updatedScreen = true;
        if (rM.updateCounter >= FULL_DISPLAY_UPDATE_QUEUE && ignoreCounter == false)
        {

            rM.updateCounter = 0;
            updateDisplay(FULL_UPDATE);
        }
        else
        {
            if (ignoreCounter == false)
            {
                rM.updateCounter += 1;
            }
            updateDisplay(PARTIAL_UPDATE);
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
    debugLog("Screen size: " + String(dis->width()) + "x" + String(dis->height()));
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
    if (resetReason != ESP_RST_POWERON && resetReason != ESP_RST_SW && resetReason != ESP_RST_DEEPSLEEP && resetReason != ESP_RST_UNKNOWN) // TODO people with v3 get rid off unknown here
    {
        return true;
    }
    else
    {
        return false;
    }
#elif ATCHY_VER == YATCHY
    if (resetReason != ESP_RST_POWERON && resetReason != ESP_RST_SW && resetReason != ESP_RST_DEEPSLEEP && resetReason != ESP_RST_USB)
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
        dis->fillScreen(GxEPD_WHITE);
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
        updateDisplay(FULL_UPDATE);
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

void updateDisplay(bool mode)
{
#if SCREEN_FULL_WHITE_WORKAROUND == 0
    // Normal
    dis->display(mode);
#else
    // The workaround
    dis->display(mode);
    if (mode == FULL_UPDATE)
    {
        delayTask(50);
        dis->display(PARTIAL_UPDATE);
    }
#endif
}