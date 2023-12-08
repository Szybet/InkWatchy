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

    initWakeUpHardware(false);
}

// Also at boot, but on wake up too
void initWakeUpHardware(bool isFromWakeUp)
{
    isDebug(Serial.begin(230400));
#if DEBUG
    if (isFromWakeUp == false)
    {
        log("Watchy is starting!");
    }
    else
    {
        log("Watchy is waking up!");
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
    log("Hardware version: " + String(HWVer));
    log("Up button pin number: " + String(UP_PIN));
    initDisplayDebug();
}

void loopHardwareDebug()
{
    loopRTCDebug();
    dumpRTCTime();
    dumpButtons();
}
#endif

TaskHandle_t motorTask;
int vibrateTime;
void vibrateMotorTaskFun(void *parameter)
{
    log("Motor on");
    digitalWrite(VIB_MOTOR_PIN, true);
    delayTask(vibrateTime);
    log("Motor off");
    digitalWrite(VIB_MOTOR_PIN, false);
    vTaskDelete(NULL);
}

void vibrateMotor(int vTime)
{
    if (vibrateMotor != 0)
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
}
