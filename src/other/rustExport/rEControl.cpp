#include "rEGeneric.h"

bool rustLimitButtons = false;

buttonState getButtonsRust()
{
    buttonState state = buttonPressed.load();
    if (state != LongUp && state != LongDown && state != LongMenu && state != None)
    {
        if (buttonPressed.compare_exchange_strong(state, None))
        {
            return state;
        }
    }
    return None;
}

void slintExit()
{
    slint_exit();
    rustLimitButtons = false;
}