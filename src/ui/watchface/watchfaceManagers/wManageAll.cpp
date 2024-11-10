#include "wManageAll.h"

RTC_DATA_ATTR uint8_t watchfaceSelected = 0;

#define WATCHFACE_COUNT 1

const watchfaceDef szybetStarfield = {
    .manager = wfmOne, 
    .name = "Szybet's starfield", 
    .data = (genPointer)&inkFieldDef,
};

const watchfaceDef *watchfacesList[WATCHFACE_COUNT] = {&szybetStarfield};

void watchfaceManageAll(bool init) {
    const watchfaceDef *watchfaceSel = watchfacesList[watchfaceSelected];
    debugLog("Watchface selected: " + String(watchfaceSel->name));

    switch(watchfaceSel->manager)
    {
        case wfmOne: {
            debugLog("wfmOne selected");
            watchfaceDefOne *wOne = (watchfaceDefOne*)watchfaceSel->data;
            wOne->drawTimeBeforeApply();
            break;
        }
        default: {
            debugLog("Watchface manager is unknown, how, why, oof");
            break;
        }
    }
}

void loopWatchfaceManage() {
    watchfaceManageAll(false);
}

void initWatchfaceManage() {
    watchfaceManageAll(true);
}