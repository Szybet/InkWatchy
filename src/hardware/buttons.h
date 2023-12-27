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
extern TaskHandle_t buttonTask;

buttonState useButtonBack();
buttonState useButton();
void useButtonBlank();
void loopButtonsTask(void *parameter);
void manageButtonWakeUp();

void initButtons();
void initButtonTask();
void setButton(buttonState button);

#if DEBUG
void dumpButtons();
String getButtonString(buttonState state);
#endif

#endif
