#include "party.h"

#if RGB_DIODE

#define PARTY_RGB 3
#define PARTY_MAX 7
uint8_t maxParty = PARTY_MAX;
uint8_t previousColor = 0;

void setRandomColor()
{
    int newColor = 0;
    while (newColor == 0 || newColor == previousColor)
    {
        newColor = betterRandom(1, maxParty + 1);
    }
    previousColor = newColor;
    setRgb((IWColors)newColor, true);
}

void initParty()
{
    dis->fillScreen(GxEPD_WHITE);
    setFont(font);
    setTextSize(1);
    writeTextCenterReplaceBack(PARTY_MESSAGE, dis->height() / 2);
    disUp(true);
}

void loopParty()
{
    setRandomColor();
    buttonState btn = useButton();
    switch (btn)
    {
    case Up:
    {
        if (maxParty == PARTY_RGB)
        {
            maxParty = PARTY_MAX;
        }
        else
        {
            maxParty = PARTY_RGB;
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

void exitParty()
{
    maxParty = 3;
    setRgb(IWColors::IwNone);
}

#endif