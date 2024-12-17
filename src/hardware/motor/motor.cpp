#include "motor.h"
#include "rtcMem.h"

TaskHandle_t motorTask = NULL;
std::mutex motorMutex;
bool motorTaskRunning = false;
int vibrateTime;

void initMotor() {
    if(bootStatus.reason != rtc && bootStatus.reason != ulp) {
        pinMode(VIB_MOTOR_PIN, OUTPUT);
        digitalWrite(VIB_MOTOR_PIN, false); // To reset the motor button if esp crashed when it was vibrating
    }
}

void vibrateMotorTaskFun(void *parameter)
{
    while (true)
    {
        debugLog("Motor on for: " + String(vibrateTime));
        motorMutex.lock();
        int vibrateTimeTmp = vibrateTime;
        vibrateTime = 0;
        motorMutex.unlock();
        analogWrite(VIB_MOTOR_PIN, VIBRATION_POWER);
        delayTask(vibrateTimeTmp);
        analogWrite(VIB_MOTOR_PIN, 0);
        debugLog("Motor off");
        // Nah, no mutex 
        if(vibrateTime == 0) {
            vTaskSuspend(NULL);
        }
    }
}

void vibrateMotor(int vTime)
{
    if (rM.disableAllVibration == true)
    {
        debugLog("Vibrations are disabled");
        if (motorTaskRunning == true)
        {
            debugLog("Deleting motor task");
            motorTaskRunning = false;
            vTaskDelete(motorTask);
        }
        return;
    }

    if (motorTaskRunning == false)
    {
        debugLog("Creating motor task");
        vibrateTime = vTime;
        motorTaskRunning = true;
        xTaskCreate(
            vibrateMotorTaskFun,
            "motorTask",
            TASK_STACK_VIBRATION,
            NULL,
            MOTOR_PRIORITY,
            &motorTask);
    }
    else
    {
        motorMutex.lock();
        vibrateTime = vTime;
        motorMutex.unlock();
        eTaskState taskState = eTaskGetState(motorTask);
        if (taskState == eSuspended)
        {
            vTaskResume(motorTask);
        }
        else
        {
            debugLog("Unknown motor task state: " + String(taskState));
        }
    }
    debugLog("Mottor task done");
}