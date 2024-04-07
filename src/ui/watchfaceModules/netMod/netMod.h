#ifndef NETMOD_H
#define NETMOD_H

#include "../../../defines/defines.h"

#if WIFI_MODULE

extern RTC_DATA_ATTR wifiStatusSimple previousWifiState;
extern wfModule wfNet;

#endif
#endif