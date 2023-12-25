#include "manager.h"

void generalSwitch(UiPlace place) {
    currentPlaceIndex += 1;
    placeTree[currentPlaceIndex] = place;
}

void switchDebugMenu() {
    generalSwitch(debugMenu);
}
