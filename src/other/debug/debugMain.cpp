#include "debugMain.h"

#if DEBUG
#include <rtcMem.h>

#if SIMPLE_DEEP_SLEEP_TEST && (WAIT_FOR_INPUT || WAIT_FOR_MONITOR)
#define IS_MCP 1
#define IS_MCP_CONFIGURATION 1
#define IS_SCREEN 1
#define IS_MOTOR 1
#define IS_RGB 1
#define IS_ACC 1
#define IS_LP_CORE 1
void simpleSleepTest()
{
#if ATCHY_VER != YATCHY
    debugLog("Only yatchy supported!");
    return;
#endif
    debugLog("simpleSleepTest start");
    delayTask(750);
    // Setup section
// Mcp23018 is present
#if IS_MCP
    rM.gpioExpander.simplerInit(false);
#if IS_MCP_CONFIGURATION
    // Note: config.h can impact what gets configured
    rM.gpioExpander.setDefaultPinStates();
#endif
#endif
#if IS_RGB
    // ALso needs MCP but hey
    // This works a fucking lot for power consumption
    setRgb(IwWhite);
    delayTask(500);
    setRgb(IwNone);
    rM.gpioExpander.setPinPullUp(RGB_DIODE_RED_PIN, true);
    rM.gpioExpander.setPinPullUp(RGB_DIODE_GREEN_PIN, true);
    rM.gpioExpander.setPinPullUp(RGB_DIODE_BLUE_PIN, true);
#endif
#if IS_SCREEN
    initDisplay();
    dis->fillCircle(100, 100, 100, SCBlack);
    dis->fillCircle(100, 100, 50, SCWhite);
    disUp(true, true, true);
#endif
#if IS_MOTOR
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    digitalWrite(VIB_MOTOR_PIN, false);
    vibrateMotor(100);
    delayTask(200);
#endif
#if IS_ACC
    initAcc();
#endif
    // Deinit section
    debugLog("Going to sleep!");
#if IS_SCREEN
    deInitScreen();

    // Not needed, as it's done before and nothing is changing
    // rM.gpioExpander.setPinState(YATCHY_DISPLAY_CS, LOW);

    initRtcInvidualGpio(EPD_RESET, RTC_GPIO_MODE_OUTPUT_ONLY);
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_RESET), true)); // Fixes high fucking power consumption - confirmed
    initRtcInvidualGpio(EPD_DC, RTC_GPIO_MODE_OUTPUT_ONLY);
    rtc_gpio_set_drive_capability(gpio_num_t(EPD_DC), GPIO_DRIVE_CAP_3); // Fixes like 2 uA
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_DC), false));      // Fixes high fucking power consumption - confirmed
    initRtcInvidualGpio(EPD_BUSY, RTC_GPIO_MODE_INPUT_ONLY);
    initRtcInvidualGpio(EPD_SPI_MOSI, RTC_GPIO_MODE_OUTPUT_ONLY);

    // Set level for MOSI and SCK break lp core, so just skip it, it was not worth anything anyway.
    // rtc_gpio_set_level with mosi to false doesn't change anything
    // ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_SPI_MOSI), true)); // Fixes like 1 uA or nothing

    initRtcInvidualGpio(EPD_SPI_SCK, RTC_GPIO_MODE_OUTPUT_ONLY);
    // rtc_gpio_set_level to true with SCK makes it maybe a bit better 1 uA things
    // ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_SPI_SCK), true));
#endif
#if IS_MCP
    deInitI2C();
#endif
#if IS_LP_CORE
    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
    loadLpCore();
    runLpCore();
#endif
    esp_deep_sleep_start();
}
#endif

void initLog()
{
#if PUT_LOGS_TO_SERIAL // This is here first because of watchy 3
    Serial.begin(SERIAL_BAUDRATE);
#if ATCHY_VER == WATCHY_3 || ATCHY_VER == YATCHY
    Serial.setDebugOutput(true);
#endif
#endif
#if WAIT_FOR_MONITOR
    delay(7500);
#endif
#if WAIT_FOR_INPUT
    int theDelay = 500;
#if WAIT_FOR_INPUT_MOTOR
    pinMode(VIB_MOTOR_PIN, OUTPUT);
#endif
    Serial.flush();
    while (true)
    {
        delayTask(theDelay / 2);
        if (Serial.available() > 0 && Serial.readString().indexOf("123") >= 0)
        {
            Serial.println("Received input! launching in 3..");
            delayTask(theDelay);
            Serial.println("2...");
            delayTask(theDelay);
            Serial.println("1...");
            delayTask(theDelay);
            Serial.println("Go!");
            break;
        }
#if WAIT_FOR_INPUT_MOTOR
        digitalWrite(VIB_MOTOR_PIN, 1);
        delayTask(theDelay / 10);
        digitalWrite(VIB_MOTOR_PIN, 0);
#endif
#if ATCHY_VER == YATCHY
        // usb_serial_jtag_driver_uninstall doesn't help
        // if it can't connect after deep sleep consider using a better usb connection
        if (usb_serial_jtag_is_connected() == false)
        {
            delayTask(1000);
        }
#endif
        Serial.println("Waiting for input: \"123\"...");
    }
#if WAIT_FOR_INPUT_MOTOR
    digitalWrite(VIB_MOTOR_PIN, 0);
#endif
#endif
    initLogs();
#if LP_CORE_TEST_ENABLED
    startLpCoreTest();
#endif
#if I2C_SCANNER
    startI2cScanner();
#endif
#if SCREENSHOT_ENDPOINT
    screenshotEndpointInit();
#endif
#if SIMPLE_DEEP_SLEEP_TEST && (WAIT_FOR_INPUT || WAIT_FOR_MONITOR)
    simpleSleepTest();
#endif
}

void showInitLogs()
{
#if DEBUG
#if DUMP_INIT_DEBUG
    initHardwareDebug();
    loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
    loopGeneralDebug();
#endif
#if VOLTAGE_PRINT_ON
    xTaskCreate(
        dumpBatteryScreen,
        "dumpBatteryTask",
        TASK_STACK_DUMP_BATTERY,
        NULL,
        0,
        nullptr);
#endif

#if SCOM_TASK_ENABLED
    initScomTask();
#endif
#endif
#if DUMP_LOOP_ACC
    initAccLog();
#endif
}

#if DUMP_LOOP_SOFTWARE_DEBUG
uint32_t peakHeap = 0;
#endif

void endLoopDebug()
{
#if DEBUG && EINK_COUNTER
    showEinkCounter();
#endif

#if DEBUG && SPEED_THROUGH_TIME
    debugLog("Speeding time");
    timeRTCUTC0.Minute = timeRTCUTC0.Minute + 1;
    if (String(timeRTCUTC0.Minute).indexOf("5") != -1)
    {
        timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
        timeRTCUTC0.Day = timeRTCUTC0.Day + 1;
        // timeRTCUTC0.Month = timeRTCUTC0.Month + 1; // We rely on previous day to clean up so this makes things break
    }
    if (timeRTCUTC0.Minute == 60)
    {
        timeRTCUTC0.Minute = 0;
        timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
    }
    if (timeRTCUTC0.Hour == 24)
    {
        timeRTCUTC0.Hour = 0;
    }
    if (timeRTCUTC0.Day == 32)
    {
        timeRTCUTC0.Day = 1;
    }
    if (timeRTCUTC0.Month == 12)
    {
        timeRTCUTC0.Month = 0;
    }
    saveRTC(timeRTCUTC0);
    return;
#endif

#if DEBUG && (DUMP_LOOP_DEBUG || DUMP_LOOP_SOFTWARE_DEBUG)
    if (millisBetter() - loopDumpDelayMs > DUMP_LOOP_DELAY)
    {
        loopDumpDelayMs = millisBetter();
#if DUMP_LOOP_DEBUG
        loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
        loopGeneralDebug();
        uint32_t heapNow = (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024;
        if(heapNow > peakHeap) {
            peakHeap = heapNow;
        }
        debugLog("Peak heap is: " + String(peakHeap));
#endif
    }
#endif
#if DUMP_LOOP_ACC
    loopAccLog();
#endif
}

void startI2cScanner()
{
#if ATCHY_VER == YATCHY
    // https://randomnerdtutorials.com/esp32-i2c-scanner-arduino/
    initI2C();
    debugLog("Entering I2C scanner loop");
    while (true)
    {
        byte error, address;
        int nDevices;
        debugLog("Scanning...");
        nDevices = 0;
        for (address = 1; address < 127; address++)
        {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();
            if (error == 0)
            {
                Serial.print("I2C device found at address 0x");
                if (address < 16)
                {
                    Serial.print("0");
                }
                Serial.println(address, HEX);
                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print("Unknow error at address 0x");
                if (address < 16)
                {
                    Serial.print("0");
                }
                Serial.println(address, HEX);
            }
        }
        if (nDevices == 0)
        {
            debugLog("No I2C devices found");
        }
        else
        {
            debugLog("done");
        }
        delayTask(5000);
    }
#else
    debugLog("I2C scanner not supported!");
#endif
}

#endif