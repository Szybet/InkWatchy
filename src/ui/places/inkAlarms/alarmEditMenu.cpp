#include "alarmEditMenu.h"

#if INK_ALARMS

inkAlarm *choosedAlarm = NULL;

void enabledAlarmChange()
{
    choosedAlarm->enabled = !choosedAlarm->enabled;
    initAlarmEditMenu();
}

void oneTimeAlarmChange()
{
    choosedAlarm->onlyOnce = !choosedAlarm->onlyOnce;
    initAlarmEditMenu();
}

void wifiAlarmChange()
{
    choosedAlarm->requireWifi = !choosedAlarm->requireWifi;
    initAlarmEditMenu();
}

bool waitingForTime = false;
void switchSetTimeHandler() {
    waitingForTime = true;
    setTimeHour = choosedAlarm->hour;
    setTimeMinute = choosedAlarm->minute;
    switchSetTime();
}

void initAlarmEditMenu()
{
    if(waitingForTime == true) {
        waitingForTime = false;
        choosedAlarm->hour = setTimeHour;
        choosedAlarm->minute = setTimeMinute;
    }

    int count = -1;
    entryMenu buttons[6];
    {
        count = count + 1;
        if (choosedAlarm->enabled == true)
        {
            buttons[count] = {ALARM_ENABLED, getImg("accept"), enabledAlarmChange};
        }
        else
        {
            buttons[count] = {ALARM_DISABLED, getImg("cross"), enabledAlarmChange};
        }
    }
    {
        count = count + 1;
        buttons[count] = {String(ALARM_TIME_PREFIX) + alarmGetTime(choosedAlarm), &emptyImgPack, switchSetTimeHandler};
    }
    {
        count = count + 1;
        if (choosedAlarm->onlyOnce == true)
        {
            buttons[count] = {ALARM_ONE_TIME, getImg("accept"), oneTimeAlarmChange};
        }
        else
        {
            buttons[count] = {ALARM_ONE_TIME, getImg("cross"), oneTimeAlarmChange};
        }
    }
    {
        if (choosedAlarm->onlyOnce == false)
        {
            count = count + 1;
            buttons[count] = {String(ALARM_DAYS_PREFIX) + alarmGetDays(choosedAlarm), &emptyImgPack, switchAlarmEditDays};
        }
    }
    {
        count = count + 1;
        if (choosedAlarm->requireWifi == true)
        {
            buttons[count] = {ALARM_REQUIRE_WIFI, getImg("accept"), wifiAlarmChange};
        }
        else
        {
            buttons[count] = {ALARM_REQUIRE_WIFI, getImg("cross"), wifiAlarmChange};
        }
    }
    count = count + 1;
    initMenu(buttons, count, ALARM_MENU_EDIT_ALARM, 1);
}

void exitAlarmEditMenu() {
    calculateNextAlarm();
}

#endif