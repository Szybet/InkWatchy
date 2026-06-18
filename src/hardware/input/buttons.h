#pragma once

#include "defines.h"

extern bool buttonsActivated;
extern std::atomic<buttonState> buttonPressed;
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
void turnOffInterrupts();
void resumeButtonTask();
extern buttonState interruptedButton;
buttonStates readButtons();
bool readOnlyOneButton(uint8_t pin); // Inneficient, if you check more than 2 buttons, use readButtons
String getButtonString(buttonState state);
