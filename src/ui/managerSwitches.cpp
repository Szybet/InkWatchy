#include "manager.h"

void generalSwitch(UiPlace place)
{
    currentPlaceIndex += 1;
    placeTree[currentPlaceIndex] = place;
    // debugLog("Current place: " + String(placeTree[currentPlaceIndex]) + " and index: " + String(currentPlaceIndex));
}

void overwriteSwitch(UiPlace place)
{
    placeTree[currentPlaceIndex] = place;
}

void switchDebugMenu()
{
    generalSwitch(debugMenu);
}

void switchSettingsMenu()
{
    generalSwitch(settingsMenu);
}

void switchWatchfaceSelectorMenu()
{
    generalSwitch(watchfaceSelector);
}

void switchGeneralDebug()
{
    generalSwitch(generalDebug);
}

void switchBatteryDebug()
{
    generalSwitch(batteryDebug);
}

void switchWifiDebug()
{
    generalSwitch(wifiDebug);
}

void switchWeatherMenu()
{
    generalSwitch(weatherMenu);
}

void switchWeatherSelectorMenu()
{
    generalSwitch(weatherConditionMenu);
}

void switchPowerMenu()
{
    generalSwitch(powerMenu);
}

void switchVault()
{
    generalSwitch(vault);
}

void switchBack()
{
    setButton(Back);
}

void switchApple()
{
    generalSwitch(apple);
}

void switchApple2()
{
    generalSwitch(apple2);
}

void switchPong()
{
    generalSwitch(pong);
}
#if CONWAY
void switchConway()
{
    generalSwitch(conwayApp);
}
#endif
#if FONT_MENU_ENABLED
void switchFontsPreview()
{
    generalSwitch(fontPreview);
}

void switchFontsPreviewMenu()
{
    generalSwitch(fontPreviewMenu);
}
#endif

void switchBook()
{
    generalSwitch(book);
}

void switchBookSelector()
{
    generalSwitch(bookSelector);
}

// call generalSwitch(textDialog); or overwriteSwitch(textDialog); before this
void showTextDialog(String str)
{
    display.fillScreen(GxEPD_WHITE);
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    simpleCenterText(str);
    disUp(true);
}