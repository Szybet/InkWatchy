#pragma once

#include "defines.h"

#if INK_ALARMS && POMODORO_ALARM

void pomodoroManage(bool force = false);
void initPomodoroMenu();

#endif