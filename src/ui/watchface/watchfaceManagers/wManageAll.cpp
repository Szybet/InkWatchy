#include "wManageAll.h"

RTC_DATA_ATTR uint8_t watchfaceSelected = 0;

#define WATCHFACE_COUNT 1

const watchfaceDef szybetStarfield = {
    .choosedManager = wfmOne, 
    .name = "Szybet's starfield", 
    .data = 0,
};

const watchfaceDef *watchfacesList[WATCHFACE_COUNT] = {&szybetStarfield};

void watchfaceManageLoop() {
    watchfaceManageAll(false);
}

void watchfaceManageInit() {
    watchfaceManageAll(true);
}

void watchfaceManageAll(bool init) {

}