#include "wifi.h"

WiFiMulti wifiMulti;

void initWifi()
{
    if (WIFI_SSID1 != "" && WIFI_PASS1 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID1));
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    }
    if (WIFI_SSID2 != "" && WIFI_PASS2 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID2));
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
    }
    if (WIFI_SSID3 != "" && WIFI_PASS3 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID3));
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
    }
    if (WIFI_SSID4 != "" && WIFI_PASS4 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID4));
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
    }
    if (WIFI_SSID5 != "" && WIFI_PASS5 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID5));
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
    }
    if (WIFI_SSID6 != "" && WIFI_PASS6 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID6));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID7 != "" && WIFI_PASS7 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID7));
        wifiMulti.addAP(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID8 != "" && WIFI_PASS8 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID8));
        wifiMulti.addAP(WIFI_SSID8, WIFI_PASS8);
    }
    if (WIFI_SSID9 != "" && WIFI_PASS9 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID9));
        wifiMulti.addAP(WIFI_SSID9, WIFI_PASS9);
    }
    if (WIFI_SSID10 != "" && WIFI_PASS10 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID10));
        wifiMulti.addAP(WIFI_SSID10, WIFI_PASS10);
    }
}

void turnOnWifi()
{
    WiFi.mode(WIFI_STA);
    //WiFi.setSleep(WIFI_PS_NONE);
    wifiMulti.run(10);
}

void turnOffWifi()
{
    WiFi.mode(WIFI_OFF);
}

#if DEBUG == 1 || DEBUG_MENUS == 1
String wifiStatus()
{
    switch (WiFi.status())
    {
    case WL_NO_SHIELD:
        return "NO SHIELD";
    case WL_IDLE_STATUS:
        return "IDLE STATUS";
    case WL_NO_SSID_AVAIL:
        return "NO SSID AVAIL";
    case WL_SCAN_COMPLETED:
        return "SCAN COMPLETED";
    case WL_CONNECTED:
        return "CONNECTED";
    case WL_CONNECT_FAILED:
        return "CONNECT FAILED";
    case WL_CONNECTION_LOST:
        return "CONNECTION LOST";
    case WL_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "UNKNOWN STATUS";
    }
}
#endif

bool isWifiConnected()
{
    // TODO: Is asking 2 times time consuming?
    log("Milis before asking time status: " + String(millis()));
    log("wifi status: " + wifiStatus());
    wl_status_t status = WiFi.status();
    log("Milis after asking time status: " + String(millis()));
    if (status == WL_CONNECTED)
    {
        return true;
    }
    else
    {
        return false;
    }
}
