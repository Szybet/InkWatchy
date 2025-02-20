#include "alarmEdit.h"

#if INK_ALARMS

void toggleDay()
{
    if (lastMenuSelected == "Monday")
    {
        toggleBit(&choosedAlarm->days, 0);
    }
    else if (lastMenuSelected == "Tuesday")
    {
        toggleBit(&choosedAlarm->days, 1);
    }
    else if (lastMenuSelected == "Wednesday")
    {
        toggleBit(&choosedAlarm->days, 2);
    }
    else if (lastMenuSelected == "Thursday")
    {
        toggleBit(&choosedAlarm->days, 3);
    }
    else if (lastMenuSelected == "Friday")
    {
        toggleBit(&choosedAlarm->days, 4);
    }
    else if (lastMenuSelected == "Saturday")
    {
        toggleBit(&choosedAlarm->days, 5);
    }
    else if (lastMenuSelected == "Sunday")
    {
        toggleBit(&choosedAlarm->days, 6);
    }
    initAlarmEditDaysMenu();
}

ImageDef *getDayEnabled(uint8_t dayID)
{
    bool enabled = getBit(choosedAlarm->days, dayID);
    if (enabled == true)
    {
        return getImg("accept");
    }
    else
    {
        return getImg("cross");
    }
}

void initAlarmEditDaysMenu()
{
    int c = 0;
    entryMenu buttons[7];
    {
        buttons[c] = {.text = "Monday", .image = getDayEnabled(0), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Tuesday", .image = getDayEnabled(1), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Wednesday", .image = getDayEnabled(2), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Thursday", .image = getDayEnabled(3), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Friday", .image = getDayEnabled(4), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Saturday", .image = getDayEnabled(5), .function = toggleDay};
        c = c + 1;
    }
    {
        buttons[c] = {.text = "Sunday", .image = getDayEnabled(6), .function = toggleDay};
        c = c + 1;
    }
    initMenu(buttons, c, "Edit days", 1);
}

void exitAlarmEditDaysMenu() {
    calculateNextAlarm();
}

#endif