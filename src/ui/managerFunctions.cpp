#include "manager.h"

void initMainMenu()
{
    entryMenu buttons[2] = {{"Debug", debugImgPack, switchDebugMenu}, {"Weather", debugImgPack, initWeatherMenu}};
    initMenu(buttons, 2, "Main menu", 1);
}

void initDebugMenu() {
    entryMenu buttons[3] = {{"General", emptyImgPack, switchGeneralDebug},{"Battery", emptyImgPack, switchBatteryDebug},{"Wifi", emptyImgPack, switchWifiDebug}};
    initMenu(buttons, 3, "Debug menu", 1);
}

void initWeatherMenu() {
    if(isWeatherAvailable == false) {
        display.fillScreen(GxEPD_WHITE);
        simpleCenterText("Weather not available");
        disUp(true);
        overwriteSwitch(textDialog);
        return void();
    }
}
