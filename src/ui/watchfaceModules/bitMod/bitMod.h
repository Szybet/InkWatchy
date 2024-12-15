#pragma once

#include "defines.h"

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
extern bool isBtcDataAvail;
extern bool isBtcDataNew;


extern wfModule wfBit;
void bitcoinSync(uint8_t tries);

#endif
