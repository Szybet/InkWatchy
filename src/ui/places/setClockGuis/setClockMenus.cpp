#include "setClockMenus.h"

#if SET_CLOCK_GUI

void switchSetClockMenu() {
    generalSwitch(setClockMenu);
}

void initSetClockMenu() {
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
        buttons[count] = {"Set timezone", &emptyImgPack, switchSetTimezoneWatch};
    }

    count = count + 1;
    initMenu(buttons, count, MENU_SETTINGS, 1);
}

#endif