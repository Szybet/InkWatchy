#pragma once

#include "defines.h"

#if WIFI_MODULE

void wfNetcheckShow(bool *showBool, bool *redrawBool);
void wfNetrequestShow(buttonState button, bool *showBool);

#endif
