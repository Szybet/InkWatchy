#include "emptyMod.h"

void wfPlacecheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
    *redrawBool = false;
}

void wfPlacerequestShow(buttonState button, bool* showBool)
{
    writeImageN(MODULE_RECT_X, MODULE_RECT_Y, getImg("module"));
    disUp(true);
}

// Lambda doesn't work here
wfModule wfPlace = {
    false,
    wfPlacecheckShow,
    wfPlacerequestShow,
};