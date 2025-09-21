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
        buttons[count] = {SETCLOCK_SET_TIME, &emptyImgPack, switchSetTimeWatch};
    }
    {
        count = count + 1;
        buttons[count] = {SETCLOCK_SET_DATE, &emptyImgPack, switchSetDateWatch};
    }
    if (strlen(TIMEZONE_OLSON) == 0 && strlen(TIMEZONE_POSIX) == 0)
    {
        count = count + 1;
        buttons[count] = {SETCLOCK_TIMEZONE_MENU, &emptyImgPack, switchSetTimezoneMenu};
    }

    count = count + 1;
    initMenu(buttons, count, SETCLOCK_CLOCK_SETTINGS, 1);
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
    showTextDialog(SETCLOCK_TIMEZONE_WARNING, false, SETCLOCK_WARNING_TITLE);
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
        buttons[count] = {String(SETCLOCK_CURRENT_TIMEZONE) + currentTimezone, &emptyImgPack, NULL};
    }
    {
        count = count + 1;
        buttons[count] = {SETCLOCK_CLEAR_TIMEZONE, &emptyImgPack, clearTimezone};
    }
    {
        count = count + 1;
        void (*theFunction)() = switchSetTimezoneWatch;
        if (setTimezoneWarningConfirmed == false)
        {
            theFunction = showTimezoneWarning;
        }
        buttons[count] = {SETCLOCK_SET_TIMEZONE, &emptyImgPack, theFunction};
    }

    count = count + 1;
    initMenu(buttons, count, SETCLOCK_TIMEZONE_SETTINGS, 1);
}

#endif