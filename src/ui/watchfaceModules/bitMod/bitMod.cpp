#include "bitMod.h"

#if BITCOIN_MODULE

#include <blockClockClient.h>

bitcoinData btcData = {0};
bool isBtcDataAvail = false;
RTC_DATA_ATTR bool isBtcDataNew = false;

void saveBitcoinData()
{
    initNvsManage();
    size_t dataSize = sizeof(bitcoinData);
    NVS.setBlob(NVS_BITCOIN, (uint8_t *)&btcData, dataSize, true);
    isBtcDataAvail = true;
}

void loadBitcoinData()
{
    initNvsManage();
    size_t dataSize = sizeof(bitcoinData);
    std::vector<uint8_t> serializedData = NVS.getBlob(NVS_BITCOIN);

    if (serializedData.size() != dataSize)
    {
        debugLog("Bitcoin data size is wrong");
        return;
    }
    memcpy(&btcData, serializedData.data(), dataSize);
    isBtcDataAvail = true;
}

void wfBitcheckShow(bool *showBool, bool *redrawBool)
{
#if MODULE_PERSISTENT
    *showBool = true;
#else
    if (isBtcDataNew == true && isBtcDataAvail == true)
    {
        *showBool = true;
        *redrawBool = true;
    }
#endif
}

#define SYNC_INFO_OFFSET 2
void wfBitrequestShow(buttonState button)
{
    debugLog("Launched");
    loadBitcoinData();
    writeImageN(MODULE_RECT_X, MODULE_RECT_Y, bitcoinImgPack);
    writeImageN(MODULE_RECT_X, MODULE_RECT_Y - PICKAXE_IMG_HEIGHT, pickaxeImgPack);
       if (isBtcDataAvail == true)
    {
        display.setCursor(MODULE_RECT_X + PICKAXE_IMG_WIDTH, MODULE_RECT_Y + MODULE_H);
        setFont(&dogicapixel4pt7b);
        setTextSize(1);
        display.print(btcData.height);
    }
    setFont(&dogicapixel4pt7b);
    setTextSize(1);
    display.setCursor(MODULE_RECT_X + MODULE_W - 60, MODULE_RECT_Y + 7); // font is 7 pixels
    display.print("Last sync");

    setFont(&dogicapixel4pt7b);
    display.setCursor(MODULE_RECT_X + MODULE_W, MODULE_RECT_Y + 7);
    String lastSync = "Never";
    if (isBtcDataAvail == true)
    {
        lastSync = unixToDate(btcData.lastSyncUnix);
    }
    uint16_t h;
    uint16_t w;
    getTextBounds(lastSync, NULL, NULL, &w, &h);
    display.setCursor(MODULE_RECT_X + MODULE_W - w - SYNC_INFO_OFFSET, MODULE_RECT_Y + 7 + h + SYNC_INFO_OFFSET);
    display.print(lastSync);
    isBtcDataNew = false;
}

wfModule wfBit = {
    false,
    wfBitcheckShow,
    wfBitrequestShow,
};

void bitcoinSync()
{
    debugLog("Launching bitcoinSync");
    BlockClockClient btcApi(COIN_LIB_API_KEY);
    String height = btcApi.getBlockHeight();
    debugLog("Got bitcoin height: " + height);

    btcData.height = height.toInt();

    if (COIN_LIB_API_KEY != "")
    {
        PriceData prices = btcApi.getBitcoinPrice();
        if (prices.error == false)
        {
            debugLog("price " + String(prices.price));
            debugLog("change1h " + String(prices.change1h));
            debugLog("change24h " + String(prices.change24h));
            debugLog("change7d " + String(prices.change7d));
            debugLog("change30d " + String(prices.change30d));
        }
        else
        {
            debugLog("Failed to get prices of btc");
        }
    }
    btcData.lastSyncUnix = getUnixTime();
    saveBitcoinData();
    isBtcDataNew = true;
}

#endif