#include "buttons.h"

int UP_PIN = 32;
uint64_t UP_MASK = GPIO_SEL_32;
buttonState buttonPressed = None;

void initButtons()
{
    // Skopiuj tu te wartosci UP_PIN i UP_MASK
    if (SRTC.getType() == PCF8563)
    {
        if (HWVer == 1.5)
        {
            UP_PIN = 32;
            UP_MASK = GPIO_SEL_32;
        }
        else
        {
            UP_PIN = 35;
            UP_MASK = GPIO_SEL_35;
        }
    }

    pinMode(MENU_PIN, INPUT);
    pinMode(BACK_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(DOWN_PIN, INPUT);
}

void buttonsLoop()
{
    if (digitalRead(BACK_PIN) == HIGH)
    {
        buttonPressed = Back;
    }
    else if (digitalRead(MENU_PIN) == HIGH && buttonPressed != Back)
    {
        buttonPressed = Menu;
    }
    else if (digitalRead(UP_PIN) == HIGH && buttonPressed != Menu && buttonPressed != Back)
    {
        buttonPressed = Up;
    }
    else if (digitalRead(DOWN_PIN) == HIGH && buttonPressed != Up && buttonPressed != Menu && buttonPressed != Back)
    {
        buttonPressed = Down;
    }
}

#if DEBUG
void dumpButtons()
{
    if (digitalRead(MENU_PIN) == 1)
    {
        log("Menu button pressed");
    }
    else if (digitalRead(BACK_PIN) == 1)
    {
        log("Back button pressed");
    }
    else if (digitalRead(UP_PIN) == 1)
    {
        log("Up button pressed");
    }
    else if (digitalRead(DOWN_PIN) == 1)
    {
        log("Down button pressed");
    }
}
#endif