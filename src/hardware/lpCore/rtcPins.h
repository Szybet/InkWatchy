#pragma once

#include "defines.h"

#if DEBUG && SIMPLE_DEEP_SLEEP_TEST
void initRtcInvidualGpio(int pin, rtc_gpio_mode_t direction);
#endif
void initRtcGpio();
void deInitRtcGpio();

