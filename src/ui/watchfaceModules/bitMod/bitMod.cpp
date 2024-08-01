#include "bitMod.h"

#if BITCOIN_MODULE

#include <blockClockClient.h>

bitcoinData btcData = {};
RTC_DATA_ATTR bool isBtcDataAvail = false;
RTC_DATA_ATTR bool isBtcDataNew = false;
RTC_DATA_ATTR bool smallBtcData = SMALL_BTC_MODULE;
RTC_DATA_ATTR uint btcLastUpdate = 0;

void saveBitcoinData()
{
    size_t dataSize = sizeof(bitcoinData);
    fsSetBlob(CONF_BITCOIN, (uint8_t *)&btcData, dataSize);
    isBtcDataAvail = true;
}

void loadBitcoinData()
{
    size_t dataSize = sizeof(bitcoinData);
    bufSize serializedData = fsGetBlob(CONF_BITCOIN);
    debugLog("bitcoin serializedData.size: " + String(serializedData.size));

    if (serializedData.size != dataSize)
    {
        debugLog("Bitcoin data size is wrong");
        isBtcDataAvail = false;
        if (serializedData.buf != NULL && serializedData.size != 0)
        {
            debugLog("Freeing bitcoin?");
            free(serializedData.buf);
        }
        return;
    }
    memcpy(&btcData, serializedData.buf, dataSize);
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
    if (isBtcDataAvail == true && (isBtcDataNew == true || getHourDifference(getUnixTime(timeRTC), btcData.lastSyncUnix) != btcLastUpdate))
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
        smallBtcData = !smallBtcData;
        clearModuleArea();
    }
    debugLog("Launched");
    loadBitcoinData();
    if (smallBtcData == true)
    {

        writeImageN(MODULE_RECT_X, MODULE_RECT_Y, getImg("bitcoin"));
        writeImageN(MODULE_RECT_X, MODULE_RECT_Y + MODULE_H - getImgWidth("pickaxe"), getImg("pickaxe"));
        if (isBtcDataAvail == true)
        {
            display.setCursor(MODULE_RECT_X + getImgWidth("pickaxe"), MODULE_RECT_Y + MODULE_H - 1);
            setFont(getFont("dogicapixel4"));
            setTextSize(1);
            display.print(btcData.height);
        }
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        display.setCursor(MODULE_RECT_X + MODULE_W - 60, MODULE_RECT_Y + 7 - 1); // font is 7 pixels
        display.print("Last sync:");

        setFont(getFont("dogicapixel4"));
        display.setCursor(MODULE_RECT_X + MODULE_W, MODULE_RECT_Y + 7);
        String lastSync = "Never";
        if (isBtcDataAvail == true)
        {
            uint diff = getHourDifference(getUnixTime(timeRTC), btcData.lastSyncUnix);
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

        if (btcData.change1h != 0.0 || btcData.change24 != 0.0 || btcData.change7d != 0.0 || btcData.change30d != 0.0 || btcData.price != 0.0)
        {
            display.setCursor(MODULE_RECT_X + getImgWidth("bitcoin"), MODULE_RECT_Y + getImgHeight("bitcoin") - 2);
            setFont(getFont("dogicapixel4"));
            setTextSize(1);
            display.print(":" + String(btcData.price) + "$");
            display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + getImgHeight("bitcoin") * 2);
            display.print("1h:" + String(btcData.change1h) + "% 24h:" + String(btcData.change24) + "%");
            display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + getImgHeight("bitcoin") * 3);
            display.print("7d:" + String(btcData.change7d) + "% 30d:" + String(btcData.change30d) + "%");
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
        display.setCursor(MODULE_RECT_X + MODULE_W - 60, MODULE_RECT_Y + 7 - 1); // font is 7 pixels
        display.print("Last sync:");

        setFont(getFont("dogicapixel4"));
        display.setCursor(MODULE_RECT_X + MODULE_W, MODULE_RECT_Y + 7);
        String lastSync = "Never";
        if (isBtcDataAvail == true)
        {
            uint diff = getHourDifference(getUnixTime(timeRTC), btcData.lastSyncUnix);
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

        // Bitclock
        display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + MODULE_H - 1);
        setFont(getFont("smileandwave20"));
        setTextSize(1);
        display.print(btcData.height);
    }

    isBtcDataNew = false;
    dUChange = true;
}

RTC_DATA_ATTR wfModule wfBit = {
#if MODULE_PERSISTENT
    true,
#else
    false,
#endif
    wfBitcheckShow,
    wfBitrequestShow,
};

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
        btcData.lastSyncUnix = getUnixTime(timeRTC);
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
        isBtcDataNew = true;
    }
    else
    {
        bitcoinSync(tries + 1);
        return;
    }
}

#endif