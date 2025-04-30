#include "debugMain.h"

#if DEBUG

void initLog()
{
#if DEBUG
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
        Serial.println("Waiting for input...");
    }
#if WAIT_FOR_INPUT_MOTOR
    digitalWrite(VIB_MOTOR_PIN, 0);
#endif
#endif
    initLogs();
#endif
#if LP_CORE_TEST_ENABLED
    startLpCoreTest();
#endif
#if I2C_SCANNER
    startI2cScanner();
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