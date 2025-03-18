#pragma once

#include "defines.h"

// Make sure this number is right, otherwise bad things will happen...
#define MODULE_COUNT 8

extern wfModule wfEmpty;

typedef enum {
    Right,
    Left,
} direction;

extern uint64_t latestModuleUpdate;

void clearModuleArea();
void nothingModule();
void drawModuleCount(bool force = false);
void wfModuleSwitch(direction where);
void wfModulesManage(buttonState button, bool forceRender = false);

