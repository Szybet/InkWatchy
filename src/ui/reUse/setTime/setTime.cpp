#include "setTime.h"

int setTimeMinute = 0;
int setTimeHour = 0;

void initSetTime()
{
    init_set_time();
    set_time_set_time(setTimeHour, setTimeMinute);
    resetSleepDelay(SLEEP_EVERY_MS);
}

void loopSetTime()
{
    slint_loop();
}

void exitSetTime()
{
    SetTimeTime time = set_time_get_time();
    setTimeHour = int(time.hour);
    setTimeMinute = int(time.minute);
    slintExit();
}