#ifndef BUTTONS_H
#define BUTTONS_H

#include "defines/defines.h"

void initButtons();
void buttonsLoop();

#if DEBUG
void dumpButtons();
#endif

typedef enum 
{
    None,
    Back,
    Menu,
    Up,
    Down,
} buttonState;

extern buttonState buttonPressed;
extern int UP_PIN;
extern uint64_t UP_MASK;

#endif