#include "wifiDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
int cursorXwifi = 0;
int StatusHeight;
int IPHeight;
int SSIDHeight;
int SignalStrengthHeight;
#define TextSize 1

uint16_t wifiStatusLength;
uint16_t ipAddressLength;
uint16_t ssidLength;
uint16_t signalLength;
String wifiStatusLastStr;

void initWifiDebugDisplay()
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);
    display.setCursor(cursorXwifi, 1);
    String menuName = "Debug Menu: Wifi";
    getTextBounds(menuName, NULL, NULL, NULL, &maxHeight);
    // maxHeight = h;

    uint16_t currentHeight = maxHeight;
    display.setCursor(cursorXwifi, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    centerText("Wifi status: ", &currentHeight);

    String wifiStatusStr = wifiStatus();
    getTextBounds(wifiStatusStr, NULL, NULL, &wifiStatusLength, NULL);
    // wifiStatusLength = w;

    centerText(wifiStatusStr, &currentHeight);
    StatusHeight = currentHeight - maxHeight;
    writeLine("IP: " + WiFi.localIP().toString(), cursorXwifi, &currentHeight);
    IPHeight = currentHeight - maxHeight;
    writeLine("SSID: " + WiFi.SSID(), cursorXwifi, &currentHeight);
    SSIDHeight = currentHeight - maxHeight;
    writeLine("Wifi signal: " + String(getSignalStrength()) + "%", cursorXwifi, &currentHeight);
    SignalStrengthHeight = currentHeight - maxHeight;

    display.fillRect(0, currentHeight - maxHeight / 2 - 2, display.width(), 1, GxEPD_BLACK);
    currentHeight = currentHeight + 5;
    writeLine("Turn wifi on", cursorXwifi, &currentHeight);
    writeLine("Turn wifi off", cursorXwifi, &currentHeight);
    display.display(FULL_UPDATE);
}

void loopWifiDebugDisplay()
{
    String wifiStatusStr = wifiStatus();
    uint16_t w;
    if (wifiStatusStr != wifiStatusLastStr)
    {
        wifiStatusStr = wifiStatus();
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        getTextBounds(wifiStatusStr, NULL, NULL, &w, NULL);
        log("w: " + String(w) + " wifiStatusLength: " + String(wifiStatusLength));
        while (w < wifiStatusLength)
        {
            wifiStatusStr = " " + wifiStatusStr + " ";
            getTextBounds(wifiStatusStr, NULL, NULL, &w, NULL);
        }
        wifiStatusLength = w;

        writeTextCenterReplaceBack(wifiStatusStr, StatusHeight);

        String ipAddressStr = "IP: " + WiFi.localIP().toString();
        uint16_t ipWidth;
        getTextBounds(ipAddressStr, NULL, NULL, &ipWidth, NULL);
        log("w: " + String(ipWidth) + " ipAddressLength: " + String(ipAddressLength));
        while (ipWidth < ipAddressLength)
        {
            ipAddressStr = " " + ipAddressStr + " ";
            getTextBounds(ipAddressStr, NULL, NULL, &ipWidth, NULL);
        }
        ipAddressLength = ipWidth;

        writeTextReplaceBack(ipAddressStr, cursorXwifi, IPHeight);

        String ssidStr = "SSID: " + WiFi.SSID();
        uint16_t ssidWidth;
        getTextBounds(ssidStr, NULL, NULL, &ssidWidth, NULL);
        log("w: " + String(ssidWidth) + " ssidLength: " + String(ssidLength));
        while (ssidWidth < ssidLength)
        {
            ssidStr = " " + ssidStr + " ";
            getTextBounds(ssidStr, NULL, NULL, &ssidWidth, NULL);
        }
        ssidLength = ssidWidth;

        writeTextReplaceBack(ssidStr, cursorXwifi, SSIDHeight);
    }
    wifiStatusLastStr = wifiStatus();

    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);

    String signalStr = "Wifi signal: " + String(getSignalStrength()) + "%";
    uint16_t signalWidth;
    getTextBounds(signalStr, NULL, NULL, &signalWidth, NULL);
    log("w: " + String(signalWidth) + " signalLength: " + String(signalLength));
    while (signalWidth < signalLength)
    {
        signalStr = " " + signalStr + " ";
        getTextBounds(signalStr, NULL, NULL, &signalWidth, NULL);
    }
    signalLength = signalWidth;

    writeTextReplaceBack(signalStr, cursorXwifi, SignalStrengthHeight);

/*
    buttonState clicked = useButton();
    if(clicked != None) {
        if(clicked == UP) {

        }
    }
    */
}
#endif
