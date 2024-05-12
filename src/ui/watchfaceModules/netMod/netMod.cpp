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
void wfNetrequestShow(buttonState button, bool *showBool)
{
    if (button == Menu)
    {
        *showBool = false;
        return;
    }
    debugLog("Launched");
    if (previousWifiState == WifiOff)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, getImg("wifiOff"));
    }
    else if (previousWifiState == WifiOn)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, getImg("wifiOn"));
    }
    else if (previousWifiState == WifiConnected)
    {
        writeImageN(WIFI_IMG_X, WIFI_IMG_Y, getImg("wifiConnected"));
        display.setCursor(MODULE_RECT_X, WIFI_IMG_Y + 25);
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        display.print(WiFi.SSID());
    }
    disUp(true);
}

// Lambda doesn't work here
RTC_DATA_ATTR wfModule wfNet = {
    false,
    wfNetcheckShow,
    wfNetrequestShow,
};
#endif