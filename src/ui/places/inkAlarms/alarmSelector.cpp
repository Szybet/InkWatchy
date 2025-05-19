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
    int c = -1;
    entryMenu buttons[MAX_ALARMS];
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        ImageDef* image = NULL;
        if(rM.alarms[i].enabled == true) {
            image = getImg("accept");
            isDebug(getUnixTimeOfAlarm(&rM.alarms[i]));
        } else {
            image = getImg("cross");
        }
        c = c + 1;
        buttons[c] = {.text = alarmNameGet(&rM.alarms[i], i), .image = image, .function = alarmSelectedHandler};
    }
    c = c + 1;
    initMenu(buttons, c, "Alarms", 1);
}

void exitAlarmSelectorMenu() {
    calculateNextAlarm();
}

// AlarmSetChooser

void initAlarmSetChooser() {
    int c = -1;
    entryMenu buttons[3];
    {
        c = c + 1;
        buttons[c] = {.text = "Browse alarms", .image = &emptyImgPack, .function = switchAlarmSelectorMenu};
    }
    {
        c = c + 1;
        buttons[c] = {.text = "Quick alarms set", .image = &emptyImgPack, .function = switchAlarmQuick};
    }
#if POMODORO_ALARM
    {
        c = c + 1;
        buttons[c] = {.text = "Pomodoro", .image = getImg("pomodoro/tomato"), .function = switchPomodoroMenu};
    }
#endif
    c = c + 1;
    initMenu(buttons, c, "Alarms", 1);
}

#endif