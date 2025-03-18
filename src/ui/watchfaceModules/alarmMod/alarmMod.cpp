#include "alarmMod.h"
#include "rtcMem.h"

#if ALARM_MODULE && INK_ALARMS

void wfAlarmcheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
    if(rM.nextAlarmIndex != -1 && rM.alarms[rM.nextAlarmIndex].enabled == true) {
        if(rM.alarms[rM.nextAlarmIndex].minute != rM.wfAlarmMinutes || rM.alarms[rM.nextAlarmIndex].hour != rM.wfAlarmHours) {
            *redrawBool = true;
            rM.wfAlarmMinutes = rM.alarms[rM.nextAlarmIndex].minute;
            rM.wfAlarmHours = rM.alarms[rM.nextAlarmIndex].hour;
        }
    } else {
        *redrawBool = false;
        rM.wfAlarmMinutes = -1;
        rM.wfAlarmHours = -1;
    }
}

void wfAlarmrequestShow(buttonState button, bool *showBool)
{
    dUChange = true;
    setFont(getFont("dogicapixel4"));
    setTextSize(1);
    squareInfo modSq = getWatchModuleSquare();
    dis->setCursor(modSq.cord.x, modSq.cord.y + 10);
    if(rM.wfAlarmMinutes == -1 && rM.wfAlarmMinutes == -1) {
        dis->print("No alarms enabled");
    } else {
        dis->print("Next alarm: ");
        dis->setCursor(modSq.cord.x, modSq.cord.y + 10 + 10);
        dis->print(alarmNameGet(&rM.alarms[rM.nextAlarmIndex], rM.nextAlarmIndex));
    }
}

#endif