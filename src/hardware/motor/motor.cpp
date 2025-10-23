#include "motor.h"
#include "rtcMem.h"

TaskHandle_t motorTask = NULL;
std::mutex motorMutex;
bool motorTaskRunning = false;
int vibrateTime;
int vibrationPower;

void initMotor()
{
    if (bootStatus.fromWakeup == false)
    {
        pinMode(VIB_MOTOR_PIN, OUTPUT);
        digitalWrite(VIB_MOTOR_PIN, false); // To reset the motor button if esp crashed when it was vibrating
    }
}

void vibrateMotorTaskFun(void *parameter)
{
    while (true)
    {
        // debugLog("Motor on for: " + String(vibrateTime));
        motorMutex.lock();
        int vibrateTimeTmp = int(vibrateTime * VIBRATION_MULTIPLIER);
        int vibratePowerTmp = vibrationPower;
        vibrateTime = 0;
        vibrationPower = 0;
        motorMutex.unlock();
        analogWrite(VIB_MOTOR_PIN, int(vibratePowerTmp * VIBRATION_MULTIPLIER));
        delayTask(vibrateTimeTmp);
        analogWrite(VIB_MOTOR_PIN, 0);
        // debugLog("Motor off");
        // Nah, no mutex
        if (vibrateTime == 0)
        {
            vTaskSuspend(NULL);
        }
    }
}

void vibrateMotor(int vTime, int power)
{
    if (rM.disableAllVibration == true)
    {
        debugLog("Vibrations are disabled");
        if (motorTaskRunning == true)
        {
            debugLog("Deleting motor task");
            motorTaskRunning = false;
            vTaskDelete(motorTask);
            motorTask = NULL;
        }
        return;
    }

    if (motorTaskRunning == false)
    {
        debugLog("Creating motor task");
        motorMutex.lock();
        vibrateTime = vTime;
        vibrationPower = power;
        motorMutex.unlock();
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
        vibrationPower = power;
        motorMutex.unlock();
        if (motorTask != NULL)
        {
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
        else
        {
            debugLog("motorTask handle is NULL");
        }
    }
    debugLog("Mottor task done");
}
