#ifndef MOTOR_H
#define MOTOR_H

#include "defines.h"

void initMotor();
extern TaskHandle_t motorTask;
extern bool motorTaskRunning;
void vibrateMotor(int vTime = VIBRATION_BUTTON_TIME);

#endif