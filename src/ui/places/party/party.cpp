#include "party.h"

#if RGB_DIODE

#define PARTY_DEFAULT 3
#define PARTY_MAX 7
uint8_t maxParty = PARTY_DEFAULT;
uint8_t previousColor = 0;

void initParty() {
    display.fillScreen(GxEPD_WHITE);
    setFont(font);
    setTextSize(1);
    writeTextCenterReplaceBack("Party party", display.height() / 2);
    disUp(true);
}

void loopParty() {
    int newColor = 0;
    while(newColor == 0 || newColor == previousColor) {
        newColor = betterRandom(1, maxParty + 1);
    }
    previousColor = newColor;
    setRgb((IWColors)newColor, false);
    buttonState btn = useButton();
    switch (btn)
    {
    case Up:
    {
        if(maxParty == PARTY_DEFAULT) {
            maxParty = PARTY_MAX;
        } else {
            maxParty = PARTY_DEFAULT;
        }
        break;
    }
    default:
    {

        break;
    }
    }
    resetSleepDelay();
    delayTask(120);
    setRgb(IWColors::IwNone, false);
}

void exitParty() {
    maxParty = 3;
    setRgb(IWColors::IwNone);
}

#endif