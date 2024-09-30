#ifndef WATCHFACE_MODULE_H
#define WATCHFACE_MODULE_H

#include "../../defines/defines.h"

// Make sure this number is right, otherwise bad things will happen...
#define MODULE_COUNT 5

#define MODULE_RECT_X 7
#define MODULE_RECT_Y 160

#define MODULE_W 177
#define MODULE_H 37

extern wfModule *wfModulesList[MODULE_COUNT];

typedef enum {
    Right,
    Left,
} direction;

extern int currentModule;
extern uint64_t latestModuleUpdate;

void clearModuleArea();
void nothingModule();
void drawModuleCount(bool force = false);
void wfModuleSwitch(direction where);
void wfModulesManage(buttonState button, bool forceRender = false);

#endif
