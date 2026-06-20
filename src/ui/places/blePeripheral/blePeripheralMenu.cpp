#include "blePeripheral.h"

#if BLE_PERIPHERAL

void initBlePeripheralMenu()
{
    int count = -1;
    entryMenu buttons[2];
#if PERIPHERAL_TOWERFALL
    {
        count = count + 1;
        buttons[count] = {"TowerFall", "", selectTowerFall};
    }
#endif
#if PERIPHERAL_BROFORCE
    {
        count = count + 1;
        buttons[count] = {"Broforce", "", selectBroforce};
    }
#endif

    count = count + 1;
    initMenu(buttons, count, "Select BLE handler", 1);
}

#endif
