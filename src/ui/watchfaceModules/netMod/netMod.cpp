#include "netMod.h"

#if WIFI_MODULE

wifiStatusSimple previousWifiState = WifiOff;

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
        dis->setCursor(modSq.cord.x, modSq.cord.y + 25);
        setFont(getFont("dogicapixel4"));
        setTextSize(1);
        dis->print(WiFi.SSID());
    }
    dUChange = true;
}

#endif