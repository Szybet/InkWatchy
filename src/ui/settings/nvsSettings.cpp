#include "nvsSettings.h"

bool RTC_DATA_ATTR disableAllVibration = false;
bool RTC_DATA_ATTR disableWakeUp = false;

void loadAllStorage() {
    disableAllVibration = bool(fsGetString(CONF_DISABLE_ALL_VIBRATION, "0"));
    disableWakeUp = bool(fsGetString(CONF_DISABLE_WAKE_UP, "0"));

    debugLog("disableAllVibration: " + BOOL_STR(disableAllVibration));
    debugLog("disableWakeUp: " + BOOL_STR(disableWakeUp));
}

void saveAllStorage() {
    fsSetString(CONF_DISABLE_ALL_VIBRATION, String(disableAllVibration));
    fsSetString(CONF_DISABLE_WAKE_UP, String(disableWakeUp));
}

void toggleAllVibration() {
    disableAllVibration = !disableAllVibration;
}

void toggleWakeUp() {
    disableWakeUp = !disableWakeUp;
}
