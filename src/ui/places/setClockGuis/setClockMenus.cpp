#include "setClockMenus.h"

#if SET_CLOCK_GUI
#include "rtcMem.h"

void switchSetClockMenu()
{
    generalSwitch(setClockMenu);
}

void initSetClockMenu()
{
    int count = -1;
    entryMenu buttons[3];
    {
        count = count + 1;
        buttons[count] = {"Set time", &emptyImgPack, switchSetTimeWatch};
    }
    {
        count = count + 1;
        buttons[count] = {"Set date", &emptyImgPack, switchSetDateWatch};
    }
    if (strlen(TIMEZONE_OLSON) == 0 && strlen(TIMEZONE_POSIX) == 0)
    {
        count = count + 1;
        buttons[count] = {"Timezone menu", &emptyImgPack, switchSetTimezoneMenu};
    }

    count = count + 1;
    initMenu(buttons, count, "Clock settings", 1);
}

void switchSetTimezoneMenu()
{
    generalSwitch(setTimezoneMenu);
}

void clearTimezone()
{
    memset(rM.posixTimeZone, '\0', POSIX_TIMEZONE_MAX_LENGTH);
    readRTC();
    initSetTimezoneMenu();
}

bool setTimezoneWarningConfirmed = false;
void showTimezoneWarning()
{
    generalSwitch(textDialog);
    showTextDialog("This menu ignores daylight/standard time and will overwrite your timezone, which may already have it.\nClick back button to confirm", false, "Warning");
    setTimezoneWarningConfirmed = true;
}

void initSetTimezoneMenu()
{
    int count = -1;
    entryMenu buttons[3];
    {
        count = count + 1;
        String currentTimezone = "none";
        if (strlen(rM.posixTimeZone) > 0)
        {
            currentTimezone = String(rM.posixTimeZone);
        }
        buttons[count] = {String("Current timezone: ") + currentTimezone, &emptyImgPack, NULL};
    }
    {
        count = count + 1;
        buttons[count] = {"Clear timezone", &emptyImgPack, clearTimezone};
    }
    {
        count = count + 1;
        void (*theFunction)() = switchSetTimezoneWatch;
        if(setTimezoneWarningConfirmed == false) {
            theFunction = showTimezoneWarning;
        }
        buttons[count] = {"Set timezone", &emptyImgPack, theFunction};
    }

    count = count + 1;
    initMenu(buttons, count, "Timezone settings", 1);
}

#endif