#pragma once

#include "defines.h"

#if EVENT_MODULE

void wfEventcheckShow(bool *showBool, bool *redrawBool);
void wfEventrequestShow(buttonState button, bool *showBool);

#endif
