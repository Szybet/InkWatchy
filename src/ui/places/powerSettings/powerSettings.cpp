#include "powerSettings.h"
#include "rtcMem.h"
#include "../../manager/manager.h"
#include "../../reUse/setTime/setTime.h"

void toggleDisableVibrationsEntry()
{
    rM.userDisableAllVibration = !rM.userDisableAllVibration;
    rM.disableAllVibration = rM.userDisableAllVibration;
    rM.isBatterySaving = false;
    initpowerMenu();
}

void toggleWakeUpEntry()
{
    rM.userDisableWakeUp = !rM.userDisableWakeUp;
    rM.disableWakeUp = rM.userDisableWakeUp;
    rM.isBatterySaving = false;
    initpowerMenu();
}

void setSleepIntervalEntry()
{
    setTimeHour = rM.daySleepForM / 60;
    setTimeMinute = rM.daySleepForM % 60;
    setTimeIsSleepInterval = true;
    switchSetTime();
}

void initpowerMenu()
{
    int count = -1;
    rM.userDisableWakeUp = rM.disableWakeUp;
    rM.userDisableAllVibration = rM.disableAllVibration;
    entryMenu buttons[3];
    {
        count = count + 1;
        String image;
        if (rM.disableAllVibration == true)
        {
            image = "accept";
        }
        else
        {
            image = "cross";
        }
        buttons[count] = {MENU_VIBRATIONS_DIS, image, toggleDisableVibrationsEntry};
    }
    {
        count = count + 1;
        String image;
        if (rM.disableWakeUp == true)
        {
            image = "accept";
        }
        else
        {
            image = "cross";
        }
        buttons[count] = {MENU_WAKE_UP_DIS, image, toggleWakeUpEntry};
    }
    {
        count = count + 1;
        buttons[count] = {MENU_SLEEP_INTERVAL, getImg("battery"), setSleepIntervalEntry};
    }
    count = count + 1;
    initMenu(buttons, count, MENU_POWER, 1);
}
