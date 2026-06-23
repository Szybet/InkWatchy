#include "combinations.h"
#include "rtcMem.h"

#define BACK_INDEX 0
#define MENU_INDEX 1
#define UP_INDEX 2
#define DOWN_INDEX 3

int8_t combinationsChecks[4];

void initCombinations()
{
    for (uint8_t i = 0; i < 4; i++)
    {
        combinationsChecks[i] = -1;
    }
}

void loopCombinations()
{
    buttonStates btns = readButtons();
    if (btns.back)
    {
        // debugLog("loopCombinations, back pin");
        combinationsChecks[BACK_INDEX] = BACK_PIN;
    }
    if (btns.menu)
    {
        // debugLog("loopCombinations, menu pin");
        combinationsChecks[MENU_INDEX] = MENU_PIN;
    }
    if (btns.up)
    {
        // debugLog("loopCombinations, up pin");
        combinationsChecks[UP_INDEX] = UP_PIN;
    }
    if (btns.down)
    {
        // debugLog("loopCombinations, down pin");
        combinationsChecks[DOWN_INDEX] = DOWN_PIN;
    }
}

bool wasClicked(uint8_t pin)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (combinationsChecks[i] == pin)
        {
            return true;
        }
    }
    return false;
}

void executeCombination()
{
    resetSleepDelay(); // Faster so it won't escape...

    bool back = wasClicked(BACK_PIN);
    bool menu = wasClicked(MENU_PIN);
    bool up = wasClicked(UP_PIN);
    bool down = wasClicked(DOWN_PIN);

    if (back && up && !menu && !down)
    {
        if (rM.currentPlace != wifiDebug)
        {
            debugLog("Executed switch wifi combination");
            switchWifiDebug();
        }
        return;
    }

#if RGB_DIODE
    if (up && down && !back && !menu)
    {
        if (currentColor != IwWhite)
        {
            setRgb(IwWhite);
        }
        else
        {
            setRgb(IwNone);
        }
        return;
    }
#endif

    // Only when in watchface
    // For now not behind any config define
    if (rM.placeTree[currentPlaceIndex] == FIRST_PLACE)
    {
        if (up && !down && !back && menu)
        {
            nextWatchface();
        }
        else if (!up && down && !back && menu)
        {
            previousWatchFace();
        }
    }
}

bool endCombinations(uint8_t currentPin)
{
    if (wasClicked(currentPin) == false)
    {
        debugLog("The current pin was not clicked");
        return false;
    }

    uint8_t count = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (combinationsChecks[i] != -1)
        {
            count = count + 1;
            if (count >= 2)
            {
                break;
            }
        }
    }
    if (count < 2)
    {
        debugLog("Less than 2 buttons were clicked");
        return false;
    }
    executeCombination();

#if DEBUG
    debugLog("So there was a combination: ");
    for (uint8_t i = 0; i < 4; i++)
    {
        debugLog("Dump combination button: " + String(combinationsChecks[i]))
    }
#endif

    return true;
}
