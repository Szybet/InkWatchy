#include "sleep.h"

void ForceInputs()
{
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
    isDebug(Serial.end());
    uint8_t P = SRTC.getADCPin();
    /* Unused GPIO PINS */
    pinMode(0, INPUT);  /*  ??      */
    pinMode(2, INPUT);  /*  ??      */
    pinMode(13, INPUT); /*  ??      */
    pinMode(15, INPUT); /*  ??      */
    pinMode(20, INPUT); /*  ??      */
    pinMode(36, INPUT); /*  ??      */
    pinMode(37, INPUT); /*  ??      */
    pinMode(38, INPUT); /*  ??      */
    pinMode(39, INPUT); /*  ??      */
                        /* RS232 */
    pinMode(3, INPUT);  /*  RX      */
                        /* BUTTONS */
    pinMode(26, INPUT); /* BTN1     */
    pinMode(25, INPUT); /* BTN2     */
    if (UP_PIN != 32)
    {
        pinMode(32, INPUT); /* BTN3     */
    }
    if (UP_PIN != 35 && P != 35)
    {
        pinMode(35, INPUT); /* ADC/BTN3 */
    }
    pinMode(4, INPUT); /* BTN4     */
                       /* BATTERY */
    // pinMode(27,INPUT);  /* INT      */
    // if (P != 33) pinMode(33,INPUT);  /* ADC      */
    // if (P != 34) pinMode(34,INPUT);  /* ADC      */
    /* RTC */
    pinMode(21, INPUT); /* SDA      */
    pinMode(22, INPUT); /* SCL      */
                        /* DISPLAY */
    pinMode(5, INPUT);  /*  CS      */
    pinMode(9, INPUT);  /* RES      */
    pinMode(10, INPUT); /*  DC      */
    pinMode(18, INPUT); /* SLCK     */
    pinMode(19, INPUT); /* BUSY     */
    pinMode(23, INPUT); /* MOSI     */
                        /* BMA 423 */
    pinMode(12, INPUT); /* INT2     */
    pinMode(14, INPUT); /* INT1     */
#elif ATCHY_VER == WATCHY_3
    // Not implemented for the watchy3! (kind of)

    // Stolen from the default firmware :)
    rtc_gpio_set_direction((gpio_num_t)UP_PIN, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pullup_en((gpio_num_t)UP_PIN);
#elif ATCHY_VER == YATCHY
    gpioExpander.deInit();
    deInitI2C();
#endif
}

void goSleep()
{
    debugLog("goSleep activated");

#if DEBUG && SCREEN_SLEEP_INFO
    display.setCursor(50, 190);
    display.setFont();
    display.setTextSize(2);
    display.print("Sleeping");
    disUp(true);
#endif

    if (buttonsActivated == true)
    {
        // deInitButtonTask(); // Should work now, reanable it later TODO
        // noInterrupts(); // Holy shit not this shit
#if ATCHY_VER != YATCHY
        detachInterrupt(UP_PIN);
        detachInterrupt(DOWN_PIN);
        detachInterrupt(BACK_PIN);
        detachInterrupt(MENU_PIN);
#else
        detachInterrupt(MCP_INTERRUPT_PIN);
#endif
    }

    // Shouldn't ever happen, as no interactions are going on
    // while (motorTaskRunning == true)
    // {
    //     debugLog("Waiting for motor task");
    //     delayTask(25);
    // }
#if LP_CORE == true
    lpCoreScreenPrepare();
    delayTask(300);
    display.hibernate();
    delayTask(100);
    initRtcGpio();
    loadLpCore();
    runLpCore();
    // This enables the subsystem, so it doesn't shut it down or something
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html#ulp-coprocessor-wakeup
    // TODO: maybe this https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html#power-down-of-rtc-peripherals-and-memories
    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
#else
    display.hibernate();
#endif

    // https://esp32.com/viewtopic.php?t=34166
    // turnOffWifi();

    // deInitWatchdogTask();
#if LP_CORE == false
    wakeUpManageRTC();
#endif

    debugLog("Going sleep...");
#if DEBUG
    logCleanup();
    Serial.print("Sleeping now!");
    flushLogs();
#endif
    ForceInputs();

    // It will always be on, how couldn't it be not
    LittleFS.end();
    // Not needed since small rtc 2.3.7
    // esp_err_t ext0Err = esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, 0);
#if ATCHY_VER != YATCHY
    esp_err_t ext1Err = esp_sleep_enable_ext1_wakeup(pinToMask(UP_PIN) | pinToMask(DOWN_PIN) | pinToMask(MENU_PIN) | pinToMask(BACK_PIN), EXT1_WAKEUP_STATE);
#else
    esp_err_t ext1Err = esp_sleep_enable_ext1_wakeup(pinToMask(MCP_INTERRUPT_PIN), EXT1_WAKEUP_STATE);
#endif
    if (ext1Err != ESP_OK)
    {
        Serial.begin(SERIAL_BAUDRATE);
        // debugLog("ext0 error: " + String(esp_err_to_name(ext0Err)));
        debugLog("ext1 error: " + String(esp_err_to_name(ext1Err)));
        delayTask(3000);
        assert("Failed to make gpio interrupts");
    }
    esp_deep_sleep_start();
}

// Should be executed in every sleep cancelation which can happen without user interaction (no buttons clicked and the watchy is woken up by RTC, then the button task is not turned on because why would it)
void checkIfButtonIsRunning()
{
    if (buttonsActivated == false)
    {
        turnOnButtons();
    }
}

void manageSleep()
{
    // debugLog("millis is:" + String(millisBetter()));
    // debugLog("sleepDelayMs is:" + String(sleepDelayMs));
    if (millisBetter() - sleepDelayMs >= SLEEP_EVERY_MS)
    {
        if (currentPlace != FIRST_PLACE)
        {
            debugLog("SLEEP_EVERY_MS runned out, Showing watchface");
            currentPlace = NoPlace;
            currentPlaceIndex = 0;
            executeExitFunc();
            setSleepDelay(TIME_FOR_WATCHFACE_TO_SHOW_MS);
        }
        else
        {
            // debugLog("isWifiTaskRunning: " + BOOL_STR(isWifiTaskCheck()));
            if (isWifiTaskCheck() == true)
            {
                debugLog("Wifi is turned on, waiting...");
                resetSleepDelay();
                checkIfButtonIsRunning();
                // debugLog("sleepDelayMs is after change:" + String(sleepDelayMs));
                return;
            }
#if WIFI_TOOL
            if (wifiToolRunning == true)
            {
                debugLog("Wifi tool is running, waiting...");
                resetSleepDelay();
                return;
            }
#endif
#if APPLE_JOKE
            if (appleJokeRunning == true)
            {
                debugLog("Apple joke is running, waiting...");
                resetSleepDelay();
                return;
            }
#endif
#if ATCHY_VER == YATCHY
            // Yatchy should never go to sleep, because of RGB diode and the interrupt switching problem (from fully charged to discharging)
            // Yes we test for both
            if (bat.isCharging == true || bat.isFullyCharged == true)
            {
                debugLog("Yatchy is charging, avoid sleep");
                isChargingCheck();
                setSleepDelay(1000);
                return;
            }
#endif
#if USB_JTAG && AVOID_SLEEP_USB_JTAG
            if (usb_serial_jtag_is_connected() == true)
            {
                debugLog("Usb jtag is connected, not going to sleep...");
                resetSleepDelay();
                return;
            }
            else
            {
                debugLog("USB jtag is not connected");
            }
#endif

#if RGB_DIODE
            rgbTaskMutex.lock();
            if (rgbTaskRunning == true)
            {
                rgbTaskMutex.unlock();
                setSleepDelay(1000);
                return;
            }
            rgbTaskMutex.unlock();
#endif

            if (buttonRead(BACK_PIN) == BUT_CLICK_STATE || buttonRead(MENU_PIN) == BUT_CLICK_STATE || buttonRead(UP_PIN) == BUT_CLICK_STATE || buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
            {
                // Basically one more watchdog test
#if WATCHDOG_TASK
                if (allButtonCheck() == true)
                {
                    debugLog("Detected all buttons high, resetting...");
                    assert(true == false);
                }
#endif
                resetSleepDelay();
                return;
            }

            uint currentSeconds = getCurrentSeconds();
            if (currentSeconds > (60 - AVOID_SLEEPING_ON_FULL_MINUTE) || wFTime.Minute != timeRTCLocal.Minute)
            {
                int toSleepSec = (((AVOID_SLEEPING_ON_FULL_MINUTE / 2) - currentSeconds + 60) % 60);
                debugLog("timeRTCLocal.Second: " + String(timeRTCLocal.Second));
                debugLog("currentSeconds: " + String(currentSeconds));
                // This message can appear a few times because watchface will attempt to force to go to sleep
                if (currentSeconds > (60 - AVOID_SLEEPING_ON_FULL_MINUTE))
                {
                    debugLog("Too near a full second! delaying it a bit. This device will wait before going to sleep in seconds: " + String(toSleepSec));
                    setSleepDelay(toSleepSec * 1000);
                }
                else
                {
                    debugLog("Minute not updated, delaying for one additional loop");
                }
                // To make sure the time updates
                if (currentSeconds < AVOID_SLEEPING_ON_FULL_MINUTE)
                {
                    readRTC();
                }
                return;
            }

#if ATCHY_VER == YATCHY
            debugLog("Battery voltage before sleep: " + String(BatteryRead()));
            debugLog("Gpio expander stat in pin state: " + BOOL_STR(gpioExpander.digitalRead(MCP_STAT_IN)));
#endif

#if DEBUG && DISABLE_SLEEP_PARTIAL
            debugLog("DISABLE_SLEEP_PARTIAL enabled, avoiding sleep");
            resetSleepDelay();
            return;
#else
            debugLog("SLEEP_EVERY_MS runned out, going to sleep");
            // resetSleepDelay(); // Not needed as we are from the loop task
            goSleep();
#endif
        }
    }
}

// https://github.com/espressif/esp-idf/blob/625bd5eb1806809ff3cc010ee20d1f750aa778a1/components/soc/include/hal/gpio_types.h#L59
// #define BIT(nr) (1UL << (nr))
uint64_t pinToMask(uint8_t pin)
{
    return ((uint64_t)(((uint64_t)1) << pin));
}