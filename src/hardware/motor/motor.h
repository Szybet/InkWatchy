#pragma once

#include "defines.h"

void initMotor();
extern TaskHandle_t motorTask;
extern bool motorTaskRunning;
void vibrateMotor(int vTime = VIBRATION_BUTTON_TIME, int power = VIBRATION_POWER);

