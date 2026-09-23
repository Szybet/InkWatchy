#include "setTime.h"
#include "rtcMem.h"

#if SET_TIME_GUI

int setTimeMinute = 0;
int setTimeHour = 0;
bool setTimeIsSleepInterval = false;

void initSetTime()
{
    if (setTimeIsSleepInterval == true)
    {
        setTimeHour = rM.daySleepForM / 60;
        setTimeMinute = rM.daySleepForM % 60;
    }

    init_set_time();
    set_time_set_time(setTimeHour, setTimeMinute);
}

void loopSetTime()
{
    resetSleepDelay(SLEEP_EVERY_MS);
    slint_loop();
}

void exitSetTime()
{
    SetTimeTime time = set_time_get_time();
    setTimeHour = int(time.hour);
    setTimeMinute = int(time.minute);

    if (setTimeIsSleepInterval == true)
    {
        int totalMinutes = setTimeHour * 60 + setTimeMinute;

        if (totalMinutes < 1)
        {
            totalMinutes = 1;
        }
        else if (totalMinutes > 1439)
        {
            totalMinutes = 1439;
        }

        rM.daySleepForM = totalMinutes;
        wakeUpManageRTC();
        setTimeIsSleepInterval = false;
    }

    slintExit();
}

#endif