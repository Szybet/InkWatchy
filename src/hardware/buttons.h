#ifndef BUTTONS_H
#define BUTTONS_H

#include "defines/defines.h"

extern std::mutex buttMut;
extern buttonState buttonPressed;
extern int UP_PIN;
extern TaskHandle_t buttonTask;

buttonState useButtonBack();
buttonState useButton();
void useButtonBlank();
void loopButtonsTask(void *parameter);
void manageButtonWakeUp();

void initButtons(bool isFromWakeUp);
void initButtonTask();
void deInitButtonTask();
void setButton(buttonState button);
void longButtonCheck(int buttonPin, buttonState normalButton, buttonState longButton);
void turnOnInterrupts();
extern buttonState interruptedButton;

#if DEBUG
void dumpButtons();
String getButtonString(buttonState state);
#endif

#endif
