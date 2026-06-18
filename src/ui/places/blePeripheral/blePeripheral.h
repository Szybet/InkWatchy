#pragma once

#include "defines.h"

#if BLE_PERIPHERAL

#include "lib/BleKeyboard.h"

extern void (*bleInputHandlerInit)(void);
extern void (*bleInputHandlerLoop)(void);
extern void (*bleInputHandlerExit)(void); // Exit can be NULL
extern BleKeyboard *bleKeyboard;
extern bool blePeripheraloneTimeOn;

void initBlePeripheral();
void loopBlePeripheral();
void exitBlePeripheral();

void initBlePeripheralMenu();

#endif
