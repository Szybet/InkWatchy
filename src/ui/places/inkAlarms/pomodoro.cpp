#include "pomodoro.h"
#include "rtcMem.h"

#if INK_ALARMS && POMODORO_ALARM

void pomodoroResetTurnOff() {
    rM.alarms[ALARM_POMODORO_ID].enabled = false;
    rM.pomodoroWorkNow = false;
    rM.pomodoroIter = 0;
    calculateNextAlarm();
    initPomodoroMenu();
}

void pomodoroManage(bool force) {
    if(rM.nextAlarmIndex == ALARM_POMODORO_ID || force == true) {
        rM.alarms[ALARM_POMODORO_ID].enabled = true;
        rM.pomodoroWorkNow = !rM.pomodoroWorkNow;
        if(rM.pomodoroWorkNow == false) {
            // So now pause
            rM.pomodoroIter = rM.pomodoroIter + 1;
            if(rM.pomodoroIter > POMODORO_LONG_PAUSE_AFTER) {
                rM.pomodoroIter = 0;
                setAlarmQuick(POMODORO_LONG_PAUSE_M, ALARM_POMODORO_ID);
            } else {
                setAlarmQuick(POMODORO_SHORT_PAUSE_M, ALARM_POMODORO_ID);
            }
        } else {
            // So now work
            setAlarmQuick(POMODORO_WORK_M, ALARM_POMODORO_ID);
        }
    }

    // TODO: is it set actually? or unset later
    
    if(rM.currentPlace == pomodoroMenu) {
        initPomodoroMenu();
    }
}

void pomodoroManageForceWrap() {
    pomodoroManage(true);
}

void initPomodoroMenu() {
    int c = -1;
    entryMenu buttons[2];
    {
        c = c + 1;
        String sta = "Status: ";
        if(rM.alarms[ALARM_POMODORO_ID].enabled == true) {
            sta = sta + "running, ";
            if(rM.pomodoroWorkNow == true) {
                sta = sta + "working, ";
            } else {
                sta = sta + "pause, ";
            }
            sta = sta + "iter: " + String(rM.pomodoroIter) + ", ";
            sta = sta + alarmGetTime(&rM.alarms[ALARM_POMODORO_ID]);
        } else {
            sta = sta + "disabled";
        }
        buttons[c] = {.text = sta, .image = getImg("pomodoro/tomato"), .function = NULL};
    }
    {
        c = c + 1;
        if(rM.alarms[ALARM_POMODORO_ID].enabled == true) {
            buttons[c] = {.text = "Reset & Turn off", .image = &emptyImgPack, .function = pomodoroResetTurnOff};
        } else {
            buttons[c] = {.text = "Start", .image = &emptyImgPack, .function = pomodoroManageForceWrap};
        }
    }
    c = c + 1;
    initMenu(buttons, c, "Pomodoro", 1);
}

#endif