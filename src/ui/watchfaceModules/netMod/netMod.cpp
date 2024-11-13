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

void wfNetrequestShow(buttonState button, bool *showBool)
{
    if (button == Menu)
    {
        *showBool = false;
        return;
    }
    debugLog("Launched");
    squareInfo modSq = getWatchModuleSquare();
    modSq.cord.x = modSq.cord.x + 1;
    modSq.cord.y = modSq.cord.y + 1;
    if (previousWifiState == WifiOff)
    {
        writeImageN(modSq.cord.x, modSq.cord.y, getImg("wifiOff"));
    }
    else if (previousWifiState == WifiOn)
    {
        writeImageN(modSq.cord.x, modSq.cord.y, getImg("wifiOn"));
    }
    else if (previousWifiState == WifiConnected)
    {
        writeImageN(modSq.cord.x, modSq.cord.y, getImg("wifiConnected"));
        display.setCursor(modSq.cord.x, modSq.cord.y + 25);
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        display.print(WiFi.SSID());
    }
    dUChange = true;
}

// Lambda doesn't work here
RTC_DATA_ATTR wfModule wfNet = {
    false,
    wfNetcheckShow,
    wfNetrequestShow,
};
#endif