#include "hardware.h"

RTC_DATA_ATTR float HWVer;
int sleepDelayMs;

// At boot, not on wake up
void initAllHardware()
{
    initRTC();
    initBattery();

    // To wake up?
    initButtons();
    
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    digitalWrite(VIB_MOTOR_PIN, false); // To reset the motor button if esp crashed when it was vibrating

    initWakeUpHardware(false);
}

// Also at boot, but on wake up too
void initWakeUpHardware(bool isFromWakeUp)
{
    isDebug(Serial.begin(115200));
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
    initDisplay(isFromWakeUp);
    resetSleepDelay();
}

void resetSleepDelay()
{
    sleepDelayMs = millis();
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
    for (int i = 0; i < vibrateTime / 2; i++)
    {
        digitalWrite(VIB_MOTOR_PIN, true);
        delayTask(1);
        digitalWrite(VIB_MOTOR_PIN, false);
        delayTask(1);
    }
    debugLog("Motor off");
    motorTaskRunning = false;
    vTaskDelete(NULL);
}

void vibrateMotor(int vTime, bool add)
{
    if (vibrateMotor != 0 && motorTaskRunning == false)
    {
        vibrateTime = vTime;
        xTaskCreate(
            vibrateMotorTaskFun,
            "motorTask",
            2000,
            NULL,
            0,
            &motorTask);
    }
    if(add == true && vibrateMotor != 0 && motorTaskRunning == true) {
        debugLog("Adding time to motor");
        vibrateTime = vibrateTime + vTime;
    }
}
