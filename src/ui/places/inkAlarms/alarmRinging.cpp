#include "alarmRinging.h"
#include "rtcMem.h"

#if INK_ALARMS

#define ALARM_TIME_X 100
#define ALARM_TIME_Y 130

uint32_t millisStart = 0;
uint32_t timeSinceLastMotor = 0;

void initAlarmRing()
{
    checkIfButtonIsRunning();
    millisStart = millisBetter();
    timeSinceLastMotor = 0;

    writeImageN(0, 0, getImg("alarms/ringingScreen"));
    setFont(getFont("UbuntuMono"));
    writeTextReplaceBack(alarmGetTime(&rM.alarms[rM.nextAlarmIndex]), ALARM_TIME_X, ALARM_TIME_Y);
    dUChange = true;
}

void loopAlarmRing()
{
    if (millisBetter() - timeSinceLastMotor > ALARM_MOTOR_DELAY)
    {
        timeSinceLastMotor = millisBetter();
        vibrateMotor(ALARM_MOTOR_TIME);
#if RGB_DIODE
        setRandomColor();
#endif
    }
    buttonState btn = useButton();
    int timeSpendAlarming = (millisBetter() - millisStart) / 1000;
    debugLog("timeSpendAlarming: " + String(timeSpendAlarming));
    if (timeSpendAlarming > ALARM_DURATION_S || btn != None)
    {
        debugLog("Alarm canceled!");
        switchBack();
    }
    disUp();
    resetSleepDelay();
}

void exitAlarmRing()
{
    if (rM.alarms[rM.nextAlarmIndex].onlyOnce == true)
    {
        rM.alarms[rM.nextAlarmIndex].onlyOnce = false;
    }
    rM.nextAlarm = 0;
    rM.nextAlarmIndex = 0;
    calculateNextAlarm();
#if RGB_DIODE
    setRgb(IwNone);
#endif
    useButtonBlank();                             // Cancel any buttons
    sleepDelayMs = sleepDelayMs - SLEEP_EVERY_MS; // Requesting sleep
}

#endif