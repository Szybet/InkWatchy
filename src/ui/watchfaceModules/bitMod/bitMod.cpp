#include "bitMod.h"
#include "rtcMem.h"

#if BITCOIN_MODULE

#include <blockClockClient.h>

bitcoinData btcData = {};

void saveBitcoinData()
{
    size_t dataSize = sizeof(bitcoinData);
    fsSetBlob(CONF_BITCOIN, (uint8_t *)&btcData, dataSize);
    rM.isBtcDataAvail = true;
}

void loadBitcoinData()
{
    size_t dataSize = sizeof(bitcoinData);
    bufSize serializedData = fsGetBlob(CONF_BITCOIN);
    debugLog("bitcoin serializedData.size: " + String(serializedData.size));

    if (serializedData.size != dataSize)
    {
        debugLog("Bitcoin data size is wrong");
        rM.isBtcDataAvail = false;
        if (serializedData.buf != NULL && serializedData.size != 0)
        {
            debugLog("Freeing bitcoin?");
            free(serializedData.buf);
        }
        return;
    }
    memcpy(&btcData, serializedData.buf, dataSize);
    rM.isBtcDataAvail = true;
}

void wfBitcheckShow(bool *showBool, bool *redrawBool)
{
#if MODULE_PERSISTENT
    *showBool = true;
#else
    if (rM.isBtcDataNew == true && rM.isBtcDataAvail == true)
    {
        *showBool = true;
    }
#endif
    // if (rM.isBtcDataAvail == true && (rM.isBtcDataNew == true || rM.btcLastUpdate + 1 != rM.btcLastUpdate) )
    if (rM.isBtcDataAvail == true && (rM.isBtcDataNew == true || getHourDifference(getUnixTime(timeRTCLocal), btcData.btcLastSyncUnix) != rM.btcLastUpdate))
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
    if (button == Down)
    {
        rM.smallBtcData = !rM.smallBtcData;
        clearModuleArea();
    }
    debugLog("Launched");
    loadBitcoinData();
    squareInfo modSq = getWatchModuleSquare();
    if (rM.smallBtcData == true)
    {

        writeImageN(modSq.cord.x, modSq.cord.y, getImg("bitcoin"));
        writeImageN(modSq.cord.x, modSq.cord.y + modSq.size.h - getImgWidth("pickaxe"), getImg("pickaxe"));
        if (rM.isBtcDataAvail == true)
        {
            dis->setCursor(modSq.cord.x + getImgWidth("pickaxe"), modSq.cord.y + modSq.size.h - 1);
            setFont(getFont("dogicapixel4"));
            setTextSize(1);
            dis->print(btcData.height);
        }
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        dis->setCursor(modSq.cord.x + modSq.size.w - 60, modSq.cord.y + 7 - 1); // font is 7 pixels
        dis->print("Last sync:");

        setFont(getFont("dogicapixel4"));
        dis->setCursor(modSq.cord.x + modSq.size.w, modSq.cord.y + 7);
        String lastSync = "Never";
        if (rM.isBtcDataAvail == true)
        {
            uint diff = getHourDifference(getUnixTime(timeRTCLocal), btcData.btcLastSyncUnix);
            // uint diff = rM.btcLastUpdate + 1;
            rM.btcLastUpdate = diff;
            debugLog("diff: " + String(diff));
            lastSync = String(diff) + "h ago";
        }
        uint16_t h;
        uint16_t w;
        getTextBounds(lastSync, NULL, NULL, &w, &h);
        dis->setCursor(modSq.cord.x + modSq.size.w - w - SYNC_INFO_OFFSET, modSq.cord.y + 7 + h + SYNC_INFO_OFFSET);
        dis->print(lastSync);

        if (btcData.change1h != 0.0 || btcData.change24 != 0.0 || btcData.change7d != 0.0 || btcData.change30d != 0.0 || btcData.price != 0.0)
        {
            dis->setCursor(modSq.cord.x + getImgWidth("bitcoin"), modSq.cord.y + getImgHeight("bitcoin") - 2);
            setFont(getFont("dogicapixel4"));
            setTextSize(1);
            dis->print(":" + String(btcData.price) + "$");
            dis->setCursor(modSq.cord.x, modSq.cord.y + getImgHeight("bitcoin") * 2);
            dis->print("1h:" + String(btcData.change1h) + "% 24h:" + String(btcData.change24) + "%");
            dis->setCursor(modSq.cord.x, modSq.cord.y + getImgHeight("bitcoin") * 3);
            dis->print("7d:" + String(btcData.change7d) + "% 30d:" + String(btcData.change30d) + "%");
        }
        else
        {
            debugLog("Not printing btc data?");
        }
    }
    else
    {
        // Sync time
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        dis->setCursor(modSq.cord.x + modSq.size.w - 60, modSq.cord.y + 7 - 1); // font is 7 pixels
        dis->print("Last sync:");

        setFont(getFont("dogicapixel4"));
        dis->setCursor(modSq.cord.x + modSq.size.w, modSq.cord.y + 7);
        String lastSync = "Never";
        if (rM.isBtcDataAvail == true)
        {
            uint diff = getHourDifference(getUnixTime(timeRTCLocal), btcData.btcLastSyncUnix);
            // uint diff = rM.btcLastUpdate + 1;
            rM.btcLastUpdate = diff;
            debugLog("diff: " + String(diff));
            lastSync = String(diff) + "h ago";
        }
        uint16_t h;
        uint16_t w;
        getTextBounds(lastSync, NULL, NULL, &w, &h);
        dis->setCursor(modSq.cord.x + modSq.size.w - w - SYNC_INFO_OFFSET, modSq.cord.y + 7 + h + SYNC_INFO_OFFSET);
        dis->print(lastSync);

        // Bitclock
        dis->setCursor(modSq.cord.x, modSq.cord.y + modSq.size.h - 1);
        setFont(getFont("smileandwave20"));
        setTextSize(1);
        dis->print(btcData.height);
    }

    rM.isBtcDataNew = false;
    dUChange = true;
}

void bitcoinSync(uint8_t tries)
{
    debugLog("Launching bitcoinSync");
    if (tries > BITCOIN_SYNC_TRIES)
    {
        debugLog("Too many tries, exiting");
        return;
    }
    BlockClockClient btcApi(COIN_LIB_API_KEY);
    String height = btcApi.getBlockHeight();
    debugLog("Got bitcoin height: " + height);

    btcData.height = height.toInt();

    if (height != 0)
    {
        btcData.btcLastSyncUnix = getUnixTime(timeRTCLocal);
        if (strlen(COIN_LIB_API_KEY) != 0)
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
        saveBitcoinData();
        rM.isBtcDataNew = true;
    }
    else
    {
        bitcoinSync(tries + 1);
        return;
    }
}

#endif