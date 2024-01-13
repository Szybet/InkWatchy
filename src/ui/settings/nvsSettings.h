#ifndef NVS_SETTING_H
#define NVS_SETTING_H

#include "../../defines/defines.h"

extern RTC_DATA_ATTR bool disableAllVibration;
extern RTC_DATA_ATTR bool disableWakeUp;

void loadAllStorage();
void saveAllStorage();

void toggleAllVibration();
void toggleWakeUp();

#endif
