#ifndef HARDWARE_H
#define HARDWARE_H

#include "defines/defines.h"
#include <GxEPD2_BW.h>
#include <SmallRTC.h>

void setupHardware();

#if DEBUG
void dumpButtons();
void showSetupResults();
#endif

extern GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
extern RTC_DATA_ATTR SmallRTC SRTC;

extern RTC_DATA_ATTR float HWVer;
extern int UP_PIN;

#endif
