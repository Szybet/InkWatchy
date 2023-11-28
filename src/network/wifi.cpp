#include "wifi.h"

WiFiManager wifiManager;

void initWifi()
{
    if (WIFI_SSID1 != "" && WIFI_PASS1 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID1));
        wifiManager.autoConnect(WIFI_SSID1, WIFI_PASS1);
    }
    if (WIFI_SSID2 != "" && WIFI_PASS2 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID2));
        wifiManager.autoConnect(WIFI_SSID2, WIFI_PASS2);
    }
    if (WIFI_SSID3 != "" && WIFI_PASS3 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID3));
        wifiManager.autoConnect(WIFI_SSID3, WIFI_PASS3);
    }
    if (WIFI_SSID4 != "" && WIFI_PASS4 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID4));
        wifiManager.autoConnect(WIFI_SSID4, WIFI_PASS4);
    }
    if (WIFI_SSID5 != "" && WIFI_PASS5 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID5));
        wifiManager.autoConnect(WIFI_SSID5, WIFI_PASS5);
    }
    if (WIFI_SSID6 != "" && WIFI_PASS6 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID6));
        wifiManager.autoConnect(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID7 != "" && WIFI_PASS7 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID7));
        wifiManager.autoConnect(WIFI_SSID7, WIFI_PASS7);
    }
    if (WIFI_SSID8 != "" && WIFI_PASS8 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID8));
        wifiManager.autoConnect(WIFI_SSID8, WIFI_PASS8);
    }
    if (WIFI_SSID9 != "" && WIFI_PASS9 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID9));
        wifiManager.autoConnect(WIFI_SSID9, WIFI_PASS9);
    }
    if (WIFI_SSID10 != "" && WIFI_PASS10 != "")
    {
        log("Attempting to connect to WiFi " + String(WIFI_SSID10));
        wifiManager.autoConnect(WIFI_SSID10, WIFI_PASS10);
    }
}

void turnOnWifi()
{
    WiFi.mode(WIFI_STA);
    wifiManager.setWiFiAutoReconnect(true);
}

void turnOffWifi()
{
    wifiManager.setWiFiAutoReconnect(false);
    wifiManager.disconnect();
    WiFi.mode(WIFI_OFF);
}

#if DEBUG == 1 || DEBUG_MENUS == 1
String wifiStatus()
{
    switch (WiFi.status())
    {
    case WL_NO_SHIELD:
        return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:
        return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    default:
        return "UNKNOWN_STATUS";
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
