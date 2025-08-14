#pragma once

#include "defines.h"

#if GADGETBRIDGE_ENABLED

void gadgetbridgeInit();
void notify(JsonDocument doc);
bool gadgetBridgeHijackSleep();

#endif