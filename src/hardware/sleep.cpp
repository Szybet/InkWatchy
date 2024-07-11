#include "sleep.h"

void ForceInputs()
{
#if ATCHY_VER == WATCHY_2
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
        detachInterrupt(UP_PIN);
        detachInterrupt(DOWN_PIN);
        detachInterrupt(BACK_PIN);
        detachInterrupt(MENU_PIN);
    }

    while (motorTaskRunning == true)
    {
        debugLog("Waiting for motor task");
        delayTask(1); // not sure
    }

    display.hibernate();

    turnOffWifi();    // To be sure only
    alarmManageRTC(); // To be sure too...
    deInitWatchdogTask();
    debugLog("Going sleep...");
#if DEBUG
    logCleanup();
    Serial.print("Sleeping now!");
    flushLogs();
    delayTask(50);
#endif
    ForceInputs();

    // This is because We need to set it at one point, but the external one will just ring from before
    // I hope I'm right and this should not be for the external one
#if RTC_TYPE == INTERNAL_RTC
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
    wakeUpManageRTC();
#endif

    LittleFS.end();
    // Not needed since small rtc 2.3.7
    // esp_err_t ext0Err = esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, 0);
    esp_err_t ext1Err = esp_sleep_enable_ext1_wakeup(pinToMask(UP_PIN) | pinToMask(DOWN_PIN) | pinToMask(MENU_PIN) | pinToMask(BACK_PIN), EXT1_WAKEUP_STATE);
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
    if (buttonTask == NULL)
    {
        initButtonTask();
    }
}

void manageSleep()
{
    // debugLog("sleepDelayMs is:" + String(sleepDelayMs));
    // debugLog("millis is:" + String(millis()));
    if (millis() - sleepDelayMs >= SLEEP_EVERY_MS)
    {
        if (currentPlace != FIRST_PLACE)
        {
            debugLog("SLEEP_EVERY_MS runned out, Showing watchface");
            currentPlace = NoPlace;
            currentPlaceIndex = 0;
            executeExitFunc();

            sleepDelayMs = sleepDelayMs + TIME_FOR_WATCHFACE_TO_SHOW_MS;
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
                return void();
            }
#if WIFI_TOOL
            if (wifiToolRunning == true)
            {
                debugLog("Wifi tool is running, waiting...");
                resetSleepDelay();
                return void();
            }
#endif
#if APPLE_JOKE
            if (appleJokeRunning == true)
            {
                debugLog("Apple joke is running, waiting...");
                resetSleepDelay();
                return void();
            }
#endif
            debugLog("SLEEP_EVERY_MS runned out, going to sleep");
            resetSleepDelay();
            goSleep();
        }
    }
}

// https://github.com/espressif/esp-idf/blob/625bd5eb1806809ff3cc010ee20d1f750aa778a1/components/soc/include/hal/gpio_types.h#L59
// #define BIT(nr) (1UL << (nr))
uint64_t pinToMask(uint8_t pin)
{
    return ((uint64_t)(((uint64_t)1) << pin));
}