#include "nvsSettings.h"

bool RTC_DATA_ATTR disableAllVibration = false;
bool RTC_DATA_ATTR disableWakeUp = false;

void loadAllStorage() {
    initNvsManage();
    //debugLog("Loading nvs storage");
    disableAllVibration = NVS.getInt(NVS_DISABLE_ALL_VIBRATION, 0);
    disableWakeUp = NVS.getInt(NVS_DISABLE_WAKE_UP, 0);
}

void saveAllStorage() {
    initNvsManage();
    //debugLog("Saving nvs storage");
    NVS.setInt(NVS_DISABLE_ALL_VIBRATION, disableAllVibration);
    NVS.setInt(NVS_DISABLE_WAKE_UP, disableWakeUp);
    NVS.commit();
}

void toggleAllVibration() {
    disableAllVibration = !disableAllVibration;
}

void toggleWakeUp() {
    disableWakeUp = !disableWakeUp;
}
