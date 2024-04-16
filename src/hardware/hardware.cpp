#include "hardware.h"

RTC_DATA_ATTR float HWVer;
long sleepDelayMs;

// Also at boot, but on wake up too
void initHardware(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
#if DEBUG
    if (isFromWakeUp == false)
    {
        debugLog("Watchy is starting!");
    }
    else
    {
        debugLog("Watchy is waking up!");
    }
#endif
#if DEBUG == 1 && DEBUG_SLOWER == 0
    setCpuMhz(maxSpeed);
#else
    // Not needed anymore, It's set in platformio.ini
    // setCpuMhz(minimalSpeed);
#endif
    initRTC(isFromWakeUp, wakeUpReason);
    initButtons(isFromWakeUp);
    if (isFromWakeUp == false)
    {
        loadAllStorage();
        initBattery();
    }
    /*
    // Not available :(
    esp_pm_config_esp32_t pm_config = {
        .max_freq_mhz = 240,        // Set the maximum CPU frequency to 80 MHz
        .min_freq_mhz = 240,        // Set the minimum CPU frequency to 40 MHz
        .light_sleep_enable = false // Enable light sleep
    };
    esp_err_t status = esp_pm_configure(&pm_config);
    debugLog("Configuring pm status: " + String(esp_err_to_name(status)));
    */
    initDisplay(isFromWakeUp);
    resetSleepDelay();

    pinMode(VIB_MOTOR_PIN, OUTPUT);
    digitalWrite(VIB_MOTOR_PIN, false); // To reset the motor button if esp crashed when it was vibrating
}

void resetSleepDelay(int addMs)
{
    sleepDelayMs = millis() + addMs;
}

#if DEBUG
void initHardwareDebug()
{
    initRTCDebug();
    debugLog("Hardware version: " + String(HWVer));
    debugLog("Up button pin number: " + String(UP_PIN));
    initDisplayDebug();
    initGeneralDebug();
}

void loopHardwareDebug()
{
    loopRTCDebug();
    dumpRTCTime();
    dumpButtons();
    dumpBattery();
    loopGeneralDebug();
}
#endif

TaskHandle_t motorTask = NULL;
bool motorTaskRunning = false;
int vibrateTime;
void vibrateMotorTaskFun(void *parameter)
{
    motorTaskRunning = true;
    debugLog("Motor on");
    for (int i = 0; i < vibrateTime / 20; i++)
    {
        digitalWrite(VIB_MOTOR_PIN, true);
        delayTask(10);
        digitalWrite(VIB_MOTOR_PIN, false);
        delayTask(10);
    }
    debugLog("Motor off");
    motorTaskRunning = false;
    vTaskDelete(NULL);
}

void vibrateMotor(int vTime, bool add)
{
    if (disableAllVibration == true)
    {
        debugLog("Vibrations are disabled");
        return void();
    }

    if (motorTaskRunning == false)
    {
        vibrateTime = vTime;
        xTaskCreate(
            vibrateMotorTaskFun,
            "motorTask",
            1500,
            NULL,
            MOTOR_PRIORITY,
            &motorTask);
    }
    if (add == true && motorTaskRunning == true)
    {
        debugLog("Adding time to motor");
        vibrateTime = vibrateTime + vTime;
    }
    debugLog("Mottor task done");
}

void setCpuMhz(cpuSpeed speed)
{
    // Only these values are available
    switch (speed)
    {
    case minimalSpeed:
    {
        setCpuFrequencyMhz(80);
        break;
    }
    case normalSpeed:
    {
        setCpuFrequencyMhz(160);
        break;
    }
    case maxSpeed:
    {
        setCpuFrequencyMhz(240);
        break;
    }
    }
}

bool isNvsInited = false;
void initNvsManage()
{
    if (isNvsInited == false)
    {
        NVS.begin();
        isNvsInited = true;
    }
}
