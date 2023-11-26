#include "wifi.h"

WiFiManager wifiManager;

bool connectToWiFi(String ssid, String password)
{
}

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

#ifdef DEBUG 
void wifiStatus () {

}
#endif