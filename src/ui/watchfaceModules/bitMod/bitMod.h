#ifndef BITMOD_H
#define BITMOD_H

#include "../../../defines/defines.h"

#if BITCOIN_MODULE

struct bitcoinData {
    int height; // Btc clock
    float price;
    float change1h;
    float change24;
    float change7d;
    float change30d;
    long lastSyncUnix;
};
extern bitcoinData btcData;
extern bool RTC_DATA_ATTR isBtcDataAvail;
extern RTC_DATA_ATTR bool isBtcDataNew;


extern RTC_DATA_ATTR wfModule wfBit;
void bitcoinSync(uint8_t tries);

#endif
#endif