#include "netMod.h"

#if WIFI_MODULE

RTC_DATA_ATTR wifiStatusSimple previousWifiState = WifiOff;

void wfNetcheckShow(bool *showBool, bool *redrawBool)
{
    wifiStatusSimple wss = wifiStatusWrap();
    if (wss != previousWifiState)
    {
        debugLog("Wifi module should be shown and redrawn");
        previousWifiState = wss;
        *showBool = true;
        *redrawBool = true;
    }
}

#define WIFI_IMG_X MODULE_RECT_X + 1
#define WIFI_IMG_Y MODULE_RECT_Y + 1
void wfNetrequestShow(buttonState button)
{
    debugLog("Launched");
    if (previousWifiState == WifiOff)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, wifiOffImgPack);
    }
    else if (previousWifiState == WifiOn)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, wifiOnImgPack);
    }
    else if (previousWifiState == WifiConnected)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, wifiConnectedImgPack);
        display.setCursor(WIFI_IMG_X, WIFI_IMG_Y + 20);
        setFont(&dogicapixel4pt7b);
        setTextSize(1);
        display.print(WiFi.SSID());
    }
    disUp(true);
}

// Lambda doesn't work here
wfModule wfNet = {
    false,
    wfNetcheckShow,
    wfNetrequestShow,
};
#endif