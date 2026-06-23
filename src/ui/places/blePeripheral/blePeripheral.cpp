#include "blePeripheral.h"

#if BLE_PERIPHERAL

void (*bleInputHandlerInit)(void) = NULL;
void (*bleInputHandlerLoop)(void) = NULL;
void (*bleInputHandlerExit)(void) = NULL;

BleKeyboard *bleKeyboard = NULL;
bool blePeripheralConnected = false;

void initBlePeripheral()
{
    if (bleInputHandlerLoop == NULL || bleInputHandlerInit == NULL)
    {
        switchBlePeripheralMenu();
        blePeripheralConnected = false;
        return;
    }

    saveCpuSpeed();
    setCpuSpeed(maxSpeed);

    debugLog("Creating new BleKeyboard");
    bleKeyboard = new BleKeyboard(BLE_NAME);
    debugLog("Launching BleKeyboard begin");
    esp_power_level_t powerLevel = ESP_PWR_LVL_P9;
#if ATCHY_VER == YATCHY
    powerLevel = ESP_PWR_LVL_P20;
#endif
    bleKeyboard->begin(powerLevel);
    debugLog("After BleKeyboard begin");

    init_general_page(2);
    general_page_set_title("Advertising BLE");
    genpage_set_center();
    genpage_add(String(String("Connect to: ") + BLE_NAME).c_str());
    genpage_add(String("Waiting...").c_str());
    general_page_set_main();
    slint_loop();
}

void loopBlePeripheral()
{
    if (!blePeripheralConnected)
    {
        if (bleKeyboard->isConnected())
        {
            slintExit();
            blePeripheralConnected = true;
            bleInputHandlerInit();
        }
        else
        {
            useButtonBlank();
        }
    }
    else
    {
        // It will try to reconnect
#if DEBUG
        if (!bleKeyboard->isConnected())
        {
            debugLog("Disconnected! bad!");
        }
#endif
        bleInputHandlerLoop();
    }
    resetSleepDelay();
}

void exitBlePeripheral()
{
    if (bleInputHandlerExit != NULL)
    {
        bleInputHandlerExit();
        bleInputHandlerExit = NULL;
    }
    if (bleKeyboard != NULL)
    {
        // End is called in destructor
        delete bleKeyboard;
        bleKeyboard = NULL;
    }
    bleInputHandlerInit = NULL;
    bleInputHandlerLoop = NULL;
    blePeripheralConnected = false;
    restoreCpuSpeed();
}

#endif
