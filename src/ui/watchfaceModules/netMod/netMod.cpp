#include "netMod.h"

RTC_DATA_ATTR wifiStatusSimple previousWifiState = WifiOff;

void wfNetcheckShow(bool *showBool, bool *redrawBool)
{
    wifiStatusSimple wss = wifiStatusWrap();
    if(wss != previousWifiState) {
        debugLog("Wifi module should be shown and redrawn");
        previousWifiState = wss;
        *showBool = true;
        *redrawBool = true;
    }
}

void wfNetrequestShow(buttonState button)
{
    // 18, 187
    display.setCursor(18, 187);
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    if(previousWifiState == WifiOff) {
        display.print("wifi off");
    } else if(previousWifiState == WifiOn) {
        display.print("wifi on");
    } else if(previousWifiState == WifiConnected) {
        display.print("wifi connected");
    }
    disUp(true);
}

// Lambda doesn't work here
wfModule wfNet = {
    false,
    wfNetcheckShow,
    wfNetrequestShow,
};