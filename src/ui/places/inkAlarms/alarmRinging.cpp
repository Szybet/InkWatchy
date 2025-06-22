#include "alarmRinging.h"
#include "rtcMem.h"

#if INK_ALARMS

#define ALARM_TIME_X 30
#define ALARM_TIME_Y 140

#define ALARM_WIFI_X 110
#define ALARM_WIFI_Y 110
#define ALARM_WIFI_SIGNAL_X 110
#define ALARM_WIFI_SIGNAL_Y 145

#define ALARM_POMODORO_X 180
#define ALARM_POMODORO_Y 20

uint32_t millisStart = 0;
uint32_t timeSinceLastMotor = 0;
uint32_t timeSinceLastWifiCheck = 0;
wifiStatusSimple previousWifi = WifiUndefined; // Cause rewrite

#if RGB_DIODE
#if POMODORO_ALARM
bool pomodoroColorIter = false;
#endif
#endif

void initAlarmRing()
{
    checkIfButtonIsRunning();
    millisStart = millisBetter();
    timeSinceLastMotor = 0;
#if ALARM_DISABLE_BACK
    disabledBacking = true;
#endif

    writeImageN(0, 0, getImg("alarms/ringingScreen"));
    setFont(getFont("UbuntuMono10"));
    writeTextReplaceBack(alarmGetTime(&rM.alarms[rM.nextAlarmIndex]), ALARM_TIME_X, ALARM_TIME_Y);

#if POMODORO_ALARM
    if (rM.nextAlarmIndex == ALARM_POMODORO_ID)
    {
        writeImageN(ALARM_POMODORO_X, ALARM_POMODORO_Y, getImg("pomodoro/tomato"));
    }
#endif

    dUChange = true;
}

void loopAlarmRing()
{
    if (millisBetter() - timeSinceLastMotor > ALARM_MOTOR_DELAY)
    {
        timeSinceLastMotor = millisBetter();
        vibrateMotor(ALARM_MOTOR_TIME);
#if RGB_DIODE
        if (rM.nextAlarmIndex != ALARM_POMODORO_ID)
        {
            setRandomColor();
        }
        else
        {
            pomodoroColorIter = !pomodoroColorIter;
            if (pomodoroColorIter == true)
            {
                setRgb(IwRed);
            }
            else
            {
                setRgb(IwNone);
            }
        }
#endif
    }
    buttonState btn = useButton();
    int timeSpendAlarming = (millisBetter() - millisStart) / 1000;
    debugLog("timeSpendAlarming: " + String(timeSpendAlarming));

    int alarmDuration = ALARM_DURATION_S;
    if (rM.alarms[rM.nextAlarmIndex].requireWifi == true)
    {
        alarmDuration = alarmDuration * ALARM_WIFI_MULTIPLIER;
        if (millisBetter() - timeSinceLastWifiCheck > 5000)
        {
            timeSinceLastWifiCheck = millisBetter();
            wifiStatusSimple wifi = wifiStatusWrap();
            if (wifi == WifiOff || wifi == WifiOn)
            {
                connectWifiQuick(alarmWifiQuick, ALARM_WIFI_WIFI_QUICK_MS);
            }
            if (wifi == WifiConnected)
            {
                int signalStrength = getSignalStrength();
                if (signalStrength >= 100)
                {
                    signalStrength = 99;
                }
                String signalStrengthStr = String(signalStrength);
                if (signalStrengthStr.length() == 1)
                {
                    signalStrengthStr = " " + signalStrengthStr;
                }
                writeTextReplaceBack(String(signalStrength) + "%", ALARM_WIFI_SIGNAL_X, ALARM_WIFI_SIGNAL_Y);
                dUChange = true;
                if (signalStrength >= ALARM_WIFI_MIN_STRENGTH)
                {
                    debugLog("Alarm canceled, because of signal strength");
#if ALARM_DISABLE_BACK
                    disabledBacking = false;
#endif
                    switchBack();
                }
            }
            if (previousWifi != wifi)
            {
                previousWifi = wifi;
                ImageDef *image = NULL;
                if (wifi == WifiConnected)
                {
                    image = getImg("wifiConnected");
                }
                else if (wifi == WifiOff)
                {
                    image = getImg("wifiOff");
                    writeTextReplaceBack(" 0%", ALARM_WIFI_SIGNAL_X, ALARM_WIFI_SIGNAL_Y);
                }
                else if (wifi == WifiOn)
                {
                    image = getImg("wifiOn");
                    writeTextReplaceBack(" 0%", ALARM_WIFI_SIGNAL_X, ALARM_WIFI_SIGNAL_Y);
                }
                writeImageN(ALARM_WIFI_X, ALARM_WIFI_Y, image);
                dUChange = true;
            }
        }
    }
    if (timeSpendAlarming > alarmDuration || (btn != None && rM.alarms[rM.nextAlarmIndex].requireWifi == false))
    {
        debugLog("Alarm canceled!");
#if ALARM_DISABLE_BACK
        disabledBacking = false;
#endif
        switchBack();
    }
    disUp();
    resetSleepDelay();
}

void exitAlarmRing()
{
#if ALARM_DISABLE_BACK
    disabledBacking = false;
#endif
    if (rM.alarms[rM.nextAlarmIndex].requireWifi == true)
    {
        disconnectWifiQuick();
    }

    if (rM.alarms[rM.nextAlarmIndex].onlyOnce == true)
    {
        rM.alarms[rM.nextAlarmIndex].enabled = false;
    }

#if POMODORO_ALARM
    if (rM.nextAlarmIndex == ALARM_POMODORO_ID)
    {
        pomodoroManage();
    }
#endif

    rM.nextAlarm = 0;
    rM.nextAlarmIndex = -1;
    calculateNextAlarm();
#if RGB_DIODE
    setRgb(IwNone);
#endif

    useButtonBlank();                             // Cancel any buttons
    sleepDelayMs = sleepDelayMs - SLEEP_EVERY_MS; // Requesting sleep
}

#endif