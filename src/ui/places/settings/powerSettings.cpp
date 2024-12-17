#include "powerSettings.h"
#include "rtcMem.h"

void loadAllStorage() {
    rM.disableAllVibration = bool(fsGetString(CONF_DISABLE_ALL_VIBRATION, "0").toInt());
    rM.disableWakeUp = bool(fsGetString(CONF_DISABLE_WAKE_UP, "0").toInt());

    debugLog("rM.disableAllVibration: " + BOOL_STR(rM.disableAllVibration));
    debugLog("rM.disableWakeUp: " + BOOL_STR(rM.disableWakeUp));
}

void saveAllStorage() {
    fsSetString(CONF_DISABLE_ALL_VIBRATION, String(rM.disableAllVibration));
    fsSetString(CONF_DISABLE_WAKE_UP, String(rM.disableWakeUp));
}

void toggleAllVibration() {
    rM.disableAllVibration = !rM.disableAllVibration;
}

void toggleWakeUp() {
    rM.disableWakeUp = !rM.disableWakeUp;
}
