#include "alarmSelector.h"
#include "rtcMem.h"

#if INK_ALARMS

void alarmSelectedHandler()
{
    choosedAlarm = &rM.alarms[currentMenuItem];
    switchAlarmEditMenu();
}

void alarmAddHandler()
{
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        if (rM.alarms[i].enabled == false)
        {
            rM.alarms[i].enabled = true;
            choosedAlarm = &rM.alarms[i];
            break;
        }
    }
    switchAlarmEditMenu();
}

void initAlarmSelectorMenu()
{
    int c = 0;
    entryMenu buttons[MAX_ALARMS + 1];
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        ImageDef* image = NULL;
        if(rM.alarms[i].enabled == true) {
            image = getImg("accept");
            isDebug(getUnixTimeOfAlarm(&rM.alarms[i]));
        } else {
            image = getImg("cross");
        }
        buttons[c] = {.text = alarmNameGet(&rM.alarms[i]), .image = image, .function = alarmSelectedHandler};
        c = c + 1;
    }

    if (c < MAX_ALARMS - 1)
    {
        buttons[c] = {.text = "Add alarm", .image = getImg("plusIcon"), .function = alarmAddHandler};
        c = c + 1;
    }

    initMenu(buttons, c, "Alarms", 1);
}

void exitAlarmSelectorMenu() {
    calculateNextAlarm();
}

#endif