#include "manager.h"

void generalSwitch(UiPlace place) {
    currentPlaceIndex += 1;
    placeTree[currentPlaceIndex] = place;
}

void overwriteSwitch(UiPlace place) {
    placeTree[currentPlaceIndex] = place;
}

void switchDebugMenu() {
    generalSwitch(debugMenu);
}

void switchGeneralDebug() {
    generalSwitch(generalDebug);
}

void switchBatteryDebug() {
    generalSwitch(batteryDebug);
}
void switchWifiDebug() {
    generalSwitch(wifiDebug);
}
