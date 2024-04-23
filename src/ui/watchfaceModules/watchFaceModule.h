#ifndef WATCHFACE_MODULE_H
#define WATCHFACE_MODULE_H

#include "../../defines/defines.h"

#define MODULE_COUNT 10

#define MODULE_RECT_X 7
#define MODULE_RECT_Y 160

#define MODULE_W 177
#define MODULE_H 37

extern RTC_DATA_ATTR wfModule *wfModulesList[MODULE_COUNT];

typedef enum {
    Right,
    Left,
} direction;

extern RTC_DATA_ATTR int currentModule;

void clearModuleArea();
void nothingModule();
void drawModuleCount(bool force = false);
void wfModuleSwitch(direction where);
void wfModulesManage(buttonState button, bool forceRender = false);

#endif
