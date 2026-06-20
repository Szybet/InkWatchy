#pragma once

#include "defines.h"

#if BLE_PERIPHERAL

#include "lib/BleKeyboard.h"

extern void (*bleInputHandlerInit)(void);
extern void (*bleInputHandlerLoop)(void);
extern void (*bleInputHandlerExit)(void); // Exit can be NULL
extern BleKeyboard *bleKeyboard;

void initBlePeripheral();
void loopBlePeripheral();
void exitBlePeripheral();

void initBlePeripheralMenu();

#if PERIPHERAL_TOWERFALL
void selectTowerFall();
#endif
#if PERIPHERAL_TOWERFALL
void selectBroforce();
#endif

#endif
