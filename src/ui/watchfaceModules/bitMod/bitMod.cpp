#include "bitMod.h"
#include "rtcMem.h"

#if BITCOIN_MODULE

#include <HTTPClient.h>

#define MEMPOOL_BASEURL "https://mempool.space/api"
#define BINANCE_BASEURL "https://api.binance.com/api/v3/klines?symbol=BTCUSDT&interval=1m&limit=1&startTime="

bitcoinData btcData = {0};

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

        if (btcData.change1h != 0.0 || btcData.change24h != 0.0 || btcData.change7d != 0.0 || btcData.change30d != 0.0 || btcData.price != 0.0)
        {
            dis->setCursor(modSq.cord.x + getImgWidth("bitcoin"), modSq.cord.y + getImgHeight("bitcoin") - 2);
            setFont(getFont("dogicapixel4"));
            setTextSize(1);
            dis->print(":" + String(btcData.price) + "$");
            dis->setCursor(modSq.cord.x, modSq.cord.y + getImgHeight("bitcoin") * 2);
            dis->print("1h:" + String(btcData.change1h) + "% 24h:" + String(btcData.change24h) + "%");
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

int getBitcoinPriceForUnix(uint64_t unixTime)
{
    HTTPClient http;
    http.addHeader("Content-Type", "application/json");
    // http.addHeader("User-Agent", "YourUserAgent");
    String binanceUrl = String(BINANCE_BASEURL) + String((unixTime - 1800) * 1000); // - 30m as we can make it for this exact second
    debugLog("Binance URL: " + binanceUrl);
    bool beginErr = http.begin(binanceUrl);
    if (beginErr == false)
    {
        debugLog("beginErr is 0");
        return 0;
    }
    int httpCode = http.GET();
    debugLog("httpCode is: " + String(httpCode));
    if (httpCode == HTTP_CODE_OK)
    {
        String output = http.getString();
        http.end();
        output = output.substring(1, output.length() - 1);
        debugLog("Received from binance: " + output);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, output);

        if (error.code() != DeserializationError::Ok)
        {
            debugLog("Failed to deserialise");
            return 0;
        }

        float two = String(doc[2]).toFloat();
        float thri = String(doc[3]).toFloat();
        debugLog("two is: " + String(two));
        int average = int(round((two + thri) / 2));
        debugLog("Final average is: " + String(average));
        return average;
    }
    http.end();
    return 0;
}

float roundToDecimal(float value, int decimalPlaces) {
    float scale = pow(10.0, decimalPlaces);
    return round(value * scale) / scale;
}

void bitcoinSync(uint8_t tries)
{
    debugLog("Launching bitcoinSync");
    if (tries > BITCOIN_SYNC_TRIES)
    {
        debugLog("Too many tries, exiting");
        return;
    }
    if(tries == 1) {
        // Zero it out
        btcData = {0};
    }

    String height = "";
    {
        HTTPClient http;
        http.begin(String(MEMPOOL_BASEURL) + "/blocks/tip/height");
        int httpCode = http.GET();
        debugLog("Height http code: " + String(httpCode));
        if (httpCode == HTTP_CODE_OK)
        {
            height = http.getString();
            http.end();
        }
        else
        {
            http.end();
            height = String("ERR ") + String(httpCode);
        }
    }

    debugLog("Got bitcoin height: " + height);

    if(btcData.height == 0) {
        btcData.height = height.toInt();
    }
    uint64_t unixTimeNow = getUnixTime(timeRTCUTC0);
    if(btcData.price == 0) {
        btcData.price = getBitcoinPriceForUnix(unixTimeNow);
    }

    int price1h = getBitcoinPriceForUnix(unixTimeNow - 3600);
    int price24h = getBitcoinPriceForUnix(unixTimeNow - 86400);
    int price7d = getBitcoinPriceForUnix(unixTimeNow - 604800);
    int price30d = getBitcoinPriceForUnix(unixTimeNow - 2592000);
    
    if(price1h != 0 && price24h != 0 && price7d != 0 && price30d != 0) {
        btcData.change1h = roundToDecimal((float(btcData.price - price1h) / float(price1h)) * 100.0, 1);
        btcData.change24h = roundToDecimal((float(btcData.price - price24h) / float(price24h)) * 100.0, 1);
        btcData.change7d = roundToDecimal((float(btcData.price - price7d) / float(price7d)) * 100.0, 1);
        btcData.change30d = roundToDecimal((float(btcData.price - price30d) / float(price30d)) * 100.0, 1);
    }

    if(btcData.height == 0 || btcData.price == 0 || price1h == 0 || price24h == 0 || price7d == 0 || price30d == 0) {
        debugLog("Running again, next try: " + String(tries + 1));
        bitcoinSync(tries + 1);
    } else {
        debugLog("Bitcoin is fine!");
        btcData.btcLastSyncUnix = getUnixTime(timeRTCLocal);
        saveBitcoinData();
    }
}

#endif