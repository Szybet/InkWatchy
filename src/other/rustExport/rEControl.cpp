#include "rEGeneric.h"

bool rustLimitButtons = false;

buttonState getButtonsRust()
{
    buttMut.lock();
    // debugLog("useButtonBack state: " + getButtonString(buttonPressed));
    if (buttonPressed != LongUp && buttonPressed != LongDown)
    {
        buttonState buttonPressedTmp = buttonPressed;
        buttonPressed = None;
        buttMut.unlock();
        return buttonPressedTmp;
    }
    buttMut.unlock();
    return None;
}

void slintExit()
{
    slintExit();
    rustLimitButtons = false;
}