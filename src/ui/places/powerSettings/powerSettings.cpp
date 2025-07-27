#include "powerSettings.h"
#include "rtcMem.h"

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

void initpowerMenu()
{
    int count = -1;
    entryMenu buttons[2];
    {
        count = count + 1;
        ImageDef *image;
        if (rM.disableAllVibration == true)
        {
            image = getImg("accept");
        }
        else
        {
            image = getImg("cross");
        }
        buttons[count] = {MENU_VIBRATIONS_DIS, image, toggleDisableVibrationsEntry};
    }
    {
        count = count + 1;
        ImageDef *image;
        if (rM.disableWakeUp == true)
        {
            image = getImg("accept");
        }
        else
        {
            image = getImg("cross");
        }
        buttons[count] = {MENU_WAKE_UP_DIS, image, toggleWakeUpEntry};
    }
    count = count + 1;
    initMenu(buttons, count, MENU_POWER, 1);
}