#include "rgb.h"

void setRgb(IWColors color)
{
#if RGB_DIODE
#if ATCHY_VER == YATCHY
    switch (color)
    {
    case IwNone:
    {
        gpioExpander.setPinState(RGB_DIODE_RED_PIN, true);
        gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, true);
        gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, true);
        return;
    }
    case IwRed:
    {
        gpioExpander.setPinState(RGB_DIODE_RED_PIN, false);
        return;
    }
    case IwGreen:
    {
        gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, false);
        return;
    }
    case IwBlue:
    {
        gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, false);
        return;
    }
    }
#endif
#endif
}