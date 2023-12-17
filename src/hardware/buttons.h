#ifndef BUTTONS_H
#define BUTTONS_H

#include "defines/defines.h"

typedef enum 
{
    None,
    Back,
    Menu,
    Up,
    Down,
    LongBack,
    LongMenu,
    LongUp,
    LongDown,
} buttonState;

extern buttonState buttonPressed;
extern int RTC_DATA_ATTR UP_PIN;
extern uint64_t RTC_DATA_ATTR UP_MASK;

buttonState useButton(bool allButtons = false);
void initButtons();
void initButtonTask();
void setButton(buttonState button);

#if DEBUG
void dumpButtons();
String getButtonString(buttonState state);
#endif

#endif
