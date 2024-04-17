#include "bitMod.h"

#if BITCOIN_MODULE

#include <blockClockClient.h>

bitcoinData btcData = {0};
RTC_DATA_ATTR bool isBtcDataAvail = false;
RTC_DATA_ATTR bool isBtcDataNew = false;
RTC_DATA_ATTR bool smallBtcData = SMALL_BTC_MODULE;
RTC_DATA_ATTR uint btcLastUpdate = 0;

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
        isBtcDataAvail = false;
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
    }
#endif
    // if (isBtcDataAvail == true && (isBtcDataNew == true || btcLastUpdate + 1 != btcLastUpdate) )
    if (isBtcDataAvail == true && (isBtcDataNew == true || getHourDifference(getUnixTime(), btcData.lastSyncUnix) != btcLastUpdate))
    {
        debugLog("btc redraw bool is true!");
        *redrawBool = true;
    }
}

#define SYNC_INFO_OFFSET 2
void wfBitrequestShow(buttonState button, bool *showBool)
{
#if MODULE_PERSISTENT == false
    if (button == Menu)
    {
        *showBool = false;
        return;
    }
#endif
    debugLog("Launched");
    loadBitcoinData();
    if (smallBtcData == true)
    {

        writeImageN(MODULE_RECT_X, MODULE_RECT_Y, bitcoinImgPack);
        writeImageN(MODULE_RECT_X, MODULE_RECT_Y + MODULE_H - PICKAXE_IMG_HEIGHT, pickaxeImgPack);
        if (isBtcDataAvail == true)
        {
            display.setCursor(MODULE_RECT_X + PICKAXE_IMG_WIDTH, MODULE_RECT_Y + MODULE_H - 1);
            setFont(&dogicapixel4pt7b);
            setTextSize(1);
            display.print(btcData.height);
        }
        setFont(&dogicapixel4pt7b);
        setTextSize(1);
        display.setCursor(MODULE_RECT_X + MODULE_W - 60, MODULE_RECT_Y + 7 - 1); // font is 7 pixels
        display.print("Last sync:");

        setFont(&dogicapixel4pt7b);
        display.setCursor(MODULE_RECT_X + MODULE_W, MODULE_RECT_Y + 7);
        String lastSync = "Never";
        if (isBtcDataAvail == true)
        {
            uint diff = getHourDifference(getUnixTime(), btcData.lastSyncUnix);
            // uint diff = btcLastUpdate + 1;
            btcLastUpdate = diff;
            debugLog("diff: " + String(diff));
            lastSync = String(diff) + "h ago";
        }
        uint16_t h;
        uint16_t w;
        getTextBounds(lastSync, NULL, NULL, &w, &h);
        display.setCursor(MODULE_RECT_X + MODULE_W - w - SYNC_INFO_OFFSET, MODULE_RECT_Y + 7 + h + SYNC_INFO_OFFSET);
        display.print(lastSync);

        if (btcData.change1h != 0.0 || btcData.change24 != 0.0 || btcData.change7d != 0.0 || btcData.change30d != 0.0 || btcData.price != 00)
        {
            display.setCursor(MODULE_RECT_X + BITCOIN_IMG_WIDTH, MODULE_RECT_Y + BITCOIN_IMG_HEIGHT - 2);
            setFont(&dogicapixel4pt7b);
            setTextSize(1);
            display.print(":" + String(btcData.price) + "$");
            display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + BITCOIN_IMG_HEIGHT * 2);
            display.print("1h:" + String(btcData.change1h) + "% 24h:" + String(btcData.change24) + "%");
            display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + BITCOIN_IMG_HEIGHT * 3);
            display.print("7d:" + String(btcData.change7d) + "% 30d:" + String(btcData.change30d) + "%");
        }
    }
    else
    {
        display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + MODULE_H - 1);
        setFont(&dimitri15pt7b);
        setTextSize(1);
        display.print(btcData.height);
    }

    isBtcDataNew = false;
    disUp(true);
}

RTC_DATA_ATTR wfModule wfBit = {
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
            btcData.price = prices.price.toFloat();
            btcData.change1h = prices.change1h;
            btcData.change24 = prices.change24h;
            btcData.change7d = prices.change7d;
            btcData.change30d = prices.change30d;
        }
        else
        {
            debugLog("Failed to get prices of btc");
            btcData.price = 0.0;
            btcData.change1h = 0.0;
            btcData.change24 = 0.0;
            btcData.change7d = 0.0;
            btcData.change30d = 0.0;
        }
    }
    else
    {
        btcData.price = 0.0;
        btcData.change1h = 0.0;
        btcData.change24 = 0.0;
        btcData.change7d = 0.0;
        btcData.change30d = 0.0;
    }
    btcData.lastSyncUnix = getUnixTime();
    saveBitcoinData();
    isBtcDataNew = true;
}

#endif