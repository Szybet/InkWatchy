#include "alarmEdit.h"

#if INK_ALARMS

void toggleDay()
{
    if (lastMenuSelected == ALARM_DAY_MONDAY)
    {
        toggleBit(&choosedAlarm->days, 0);
    }
    else if (lastMenuSelected == ALARM_DAY_TUESDAY)
    {
        toggleBit(&choosedAlarm->days, 1);
    }
    else if (lastMenuSelected == ALARM_DAY_WEDNESDAY)
    {
        toggleBit(&choosedAlarm->days, 2);
    }
    else if (lastMenuSelected == ALARM_DAY_THURSDAY)
    {
        toggleBit(&choosedAlarm->days, 3);
    }
    else if (lastMenuSelected == ALARM_DAY_FRIDAY)
    {
        toggleBit(&choosedAlarm->days, 4);
    }
    else if (lastMenuSelected == ALARM_DAY_SATURDAY)
    {
        toggleBit(&choosedAlarm->days, 5);
    }
    else if (lastMenuSelected == ALARM_DAY_SUNDAY)
    {
        toggleBit(&choosedAlarm->days, 6);
    }
    initAlarmEditDaysMenu();
}

String getDayEnabled(uint8_t dayID)
{
    bool enabled = getBit(choosedAlarm->days, dayID);
    if (enabled == true)
    {
        return "accept";
    }
    else
    {
        return "cross";
    }
}

void initAlarmEditDaysMenu()
{
    int c = 0;
    entryMenu buttons[7];
    {
        buttons[c] = {.text = ALARM_DAY_MONDAY, .imageKey = getDayEnabled(0), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_TUESDAY, .imageKey = getDayEnabled(1), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_WEDNESDAY, .imageKey = getDayEnabled(2), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_THURSDAY, .imageKey = getDayEnabled(3), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_FRIDAY, .imageKey = getDayEnabled(4), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_SATURDAY, .imageKey = getDayEnabled(5), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = ALARM_DAY_SUNDAY, .imageKey = getDayEnabled(6), .function = toggleDay};
        c = c + 1;
    }
    initMenu(buttons, c, ALARM_MENU_EDIT_DAYS, 1);
}

void exitAlarmEditDaysMenu() {
    calculateNextAlarm();
}

#endif
