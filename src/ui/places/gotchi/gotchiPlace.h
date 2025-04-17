#pragma once

#include "defines.h"

#if GOTCHI

extern std::mutex gotchiBuffMutex;
extern GFXcanvas1 *gotchiBuff;

#define BUTTON_TICKS_NEEDED 200
struct gotchiButtons
{
    uint16_t left;
    uint16_t right;
    uint16_t middle;
};

extern gotchiButtons gotchiButtonsEx; // Exchange

void startGotchiTask();
void endGotchiTask();

void initGotchi();
void loopGotchi();
void exitGotchi();

#endif