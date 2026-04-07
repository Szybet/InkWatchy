#include "BLEHostScanner.h"

#if BLE_HOST_ENABLED

void showTextSimplify()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        slint_loop();
    }
}

void onDeviceSelected()
{
    int selected = currentMenuItem;
    debugLog("Selected device at index: " + String(selected));
    // We set it early, no worries
    hostBleClientName = hostBleGetScannedDeviceName(selected);
    showTextDialog("Connecting to " + hostBleClientName + "...", true, "BLE scanner");
    showTextSimplify();
    bool connected = hostBleConnectToDevice(selected);
    if(connected == true) {
        showTextDialog("Connected to " + hostBleClientName, true, "BLE scanner");
        showTextSimplify();
    } else {
        showTextDialog("Failed to connect to " + hostBleClientName, true, "BLE scanner");
        showTextSimplify();
    }
    // Not ideal
    delayTask(750);
    switchBack();
}

bool hostScannerMenu = true;
void initBluetoothHostScanner()
{
    debugLog("Init bluetooth host scanner called");
    showTextDialog("Scanning...", true, "BLE scanner");
    showTextSimplify();

    resetSleepDelay(SLEEP_EVERY_MS);

    hostBleInitClient();
    hostBleStartScan(5);

    int count = hostBleGetScannedDevicesCount();
    debugLog("Found " + String(count) + " devices");

    if (count == 0)
    {
        showTextDialog("No devices found", true, "BLE scanner");
        hostScannerMenu = false;
        return;
    }

    entryMenu *devices = new entryMenu[count];
    for (int i = 0; i < count; i++)
    {
        String name = hostBleGetScannedDeviceName(i);
        devices[i] = {name, &emptyImgPack, onDeviceSelected};
    }

    hostScannerMenu = true;
    initMenu(devices, count, "Choose device");
}

void exitBluetoothHostScanner()
{
    debugLog("Exit bluetooth host scanner called");
}

void loopBluetoothHostScanner()
{
    if (hostScannerMenu == true)
    {
        loopMenu();
    }
    else
    {
        slint_loop();
    }
}

#endif
