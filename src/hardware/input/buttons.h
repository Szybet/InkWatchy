#ifndef BUTTONS_H
#define BUTTONS_H

#include "defines.h"

extern bool buttonsActivated;
extern std::mutex buttMut;
extern buttonState buttonPressed;
extern TaskHandle_t buttonTask;

buttonState useButtonBack();
buttonState useButton();
buttonState useAllButtons();
void useButtonBlank();
void loopButtonsTask(void *parameter);
void manageButtonWakeUp();

void turnOnButtons();
void initButtons();
void initButtonTask();
void deInitButtonTask();
void setButton(buttonState button);
void longButtonCheck(int buttonPin, buttonState normalButton, buttonState longButton);
void turnOnInterrupts();
void resumeButtonTask();
extern buttonState interruptedButton;
bool buttonRead(uint8_t pin); // Wrapper for digitalRead for buttons

#if DEBUG
void dumpButtons();
String getButtonString(buttonState state);
#endif

#endif
