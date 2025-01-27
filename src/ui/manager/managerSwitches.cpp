#include "manager.h"
#include "rtcMem.h"

void generalSwitch(UiPlace place)
{
    currentPlaceIndex += 1;
    rM.placeTree[currentPlaceIndex] = place;
    // debugLog("Current place: " + String(rM.placeTree[currentPlaceIndex]) + " and index: " + String(currentPlaceIndex));
}

void overwriteSwitch(UiPlace place)
{
    rM.placeTree[currentPlaceIndex] = place;
}

void switchDebugMenu()
{
    generalSwitch(debugMenu);
}

void switchSettingsMenu()
{
    generalSwitch(settingsMenu);
}

void switchGamesMenu()
{
    generalSwitch(gamesMenu);
}

void switchWatchfaceSelectorMenu()
{
    generalSwitch(watchfaceSelector);
}

void switchGeneralDebug()
{
    generalSwitch(generalDebug);
}

void switchClockDebug()
{
    generalSwitch(clockDebug);
}

void switchBatteryDebug()
{
    generalSwitch(batteryDebug);
}

void switchWifiDebug()
{
    generalSwitch(wifiDebug);
}

void switchGitDebug()
{
    generalSwitch(gitDebug);
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

#if VIDEO_PLAYER
void switchVideoPlayer()
{
    generalSwitch(videoPlayer);
}

void switchVideoMenu()
{
    generalSwitch(videoMenu);
}
#endif

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

#if TETRIS
void switchTetris()
{
    generalSwitch(tetris);
}
#endif

#if CREDITS
void switchCredits()
{
    generalSwitch(credits);
}
#endif

#if CONWAY
void switchConway()
{
    generalSwitch(conwayApp);
}
#endif

#if RGB_DIODE
void switchParty()
{
    generalSwitch(partyApp);
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
    dis->fillScreen(GxEPD_WHITE);
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    simpleCenterText(str);
    disUp(true);
}