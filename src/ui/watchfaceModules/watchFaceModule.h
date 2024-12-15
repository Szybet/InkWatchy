#pragma once

#include "defines.h"

// Make sure this number is right, otherwise bad things will happen...
#define MODULE_COUNT 7

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

