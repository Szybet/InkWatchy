#include "rgb.h"

void setRgb(IWColors color, bool clearPrevious)
{
#if RGB_DIODE
#if ATCHY_VER == YATCHY
    if (gpioExpander.simplerInit() == false)
    {
        return;
    }
    if (clearPrevious == true && color != IwNone)
    {
        setRgb(IwNone, false);
    }
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
    case IwYellow:
    {
        setRgb(IwRed, true);
        setRgb(IwGreen, false);
        return;
    }
    case IwPink:
    {
        setRgb(IwRed, true);
        setRgb(IwBlue, false);
        return;
    }
    case IwCyan:
    {
        setRgb(IwGreen, true);
        setRgb(IwBlue, false);
        return;
    }
    case IwWhite:
    {
        setRgb(IwGreen, true);
        setRgb(IwBlue, false);
        setRgb(IwGreen, false);
        return;
    }
    }
#endif
#endif
}