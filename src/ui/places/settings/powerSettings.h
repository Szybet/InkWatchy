#ifndef POWER_SETTING_H
#define POWER_SETTING_H

#include "defines.h"

extern bool disableAllVibration;
extern bool disableWakeUp;

void loadAllStorage();
void saveAllStorage();

void toggleAllVibration();
void toggleWakeUp();

#endif
