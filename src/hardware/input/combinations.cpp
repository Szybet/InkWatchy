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
#if ATCHY_VER != YATCHY
    if (buttonRead(BACK_PIN) == BUT_CLICK_STATE)
    {
        // debugLog("loopCombinations, back pin");
        combinationsChecks[BACK_INDEX] = BACK_PIN;
    }
    if (buttonRead(MENU_PIN) == BUT_CLICK_STATE)
    {
        // debugLog("loopCombinations, menu pin");
        combinationsChecks[MENU_INDEX] = MENU_PIN;
    }
    if (buttonRead(UP_PIN) == BUT_CLICK_STATE)
    {
        // debugLog("loopCombinations, up pin");
        combinationsChecks[UP_INDEX] = UP_PIN;
    }
    if (buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        // debugLog("loopCombinations, down pin");
        combinationsChecks[DOWN_INDEX] = DOWN_PIN;
    }
#else
    // Optimise + avoid i2c error
    uint16_t reg = rM.gpioExpander.readRegister(MCP_GPIO);
    bool backBtn = !rM.gpioExpander.checkBit(reg, BACK_PIN);
    bool menuBtn = !rM.gpioExpander.checkBit(reg, MENU_PIN);
    bool downBtn = !rM.gpioExpander.checkBit(reg, DOWN_PIN);
    bool upBtn = !rM.gpioExpander.checkBit(reg, UP_PIN);

    if (backBtn && menuBtn && downBtn && upBtn)
    {
        debugLog("Possible I2C error, all buttons clicked");
        return;
    }

    if (backBtn)
    {
        combinationsChecks[BACK_INDEX] = BACK_PIN;
    }
    if (menuBtn)
    {
        combinationsChecks[MENU_INDEX] = MENU_PIN;
    }
    if (upBtn)
    {
        combinationsChecks[UP_INDEX] = UP_PIN;
    }
    if (downBtn)
    {
        combinationsChecks[DOWN_INDEX] = DOWN_PIN;
    }
#endif
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
    if (wasClicked(BACK_PIN) == true && wasClicked(UP_PIN) == true && wasClicked(MENU_PIN) == false && wasClicked(DOWN_PIN) == false)
    {
        if (rM.currentPlace != wifiDebug)
        {
            debugLog("Executed switch wifi combination");
            switchWifiDebug();
        }
        return;
    }
#if RGB_DIODE
    if (wasClicked(UP_PIN) == true && wasClicked(DOWN_PIN) == true && wasClicked(BACK_PIN) == false && wasClicked(MENU_PIN) == false)
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
