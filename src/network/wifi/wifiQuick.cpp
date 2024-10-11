#include "wifiQuick.h"

// Stack overflow stolen yes
void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base)
{
    for (int i = 0; i < maxBytes; i++)
    {
        bytes[i] = strtoul(str, NULL, base); // Convert byte
        str = strchr(str, sep);              // Find next separator
        if (str == NULL || *str == '\0')
        {
            break; // No more separators, exit
        }
        str++; // Point to next character after separator
    }
}

IPAddress wqIp(WQ_STAT_IP);
IPAddress wqGateway(WQ_GATEWAY);
IPAddress wqSubnet(WQ_SUBNET);
IPAddress wqDns1(1, 1, 1, 1);
IPAddress wqDns2(8, 8, 4, 4);

#if DEBUG
int64_t startMill = 0;
#endif
cpuSpeed cs;

bool connectWifiQuick()
{
#if DEBUG
    debugLog("Turning on wifi quick");
    startMill = millisBetter();
#endif
    uint8_t mac[6];
    parseBytes(WQ_BSSID, ':', mac, 6, 16);

    // Doesn't change anything really, maybe tcp
    cs = getCpuSpeed();
    setCpuSpeed(maxSpeed);
    WiFi.setSleep(WIFI_PS_NONE);

    if (!WiFi.config(wqIp, wqGateway, wqSubnet, wqDns1, wqDns2))
    {
        debugLog("Failed to configure wq");
        return false;
    }

    // Channel to 0 if unknown
    // Empty mac as (const uint8_t *)__null
    WiFi.begin(WQ_SSID, WQ_PASS, 0, mac);

    int delay = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        debugLog("Waiting for connecting...");
        delay = delay + WIFI_QUICK_CHECK_MS;
        if (delay > WIFI_QUICK_MAX_MS)
        {
            debugLog("Failed to connect to quick wifi");
            turnOffWifiMinimal();
            // setCpuSpeed(cs);
            return false;
        }
        delayTask(WIFI_QUICK_CHECK_MS);
    }

#if DEBUG
    debugLog("Connected to wifi, it took: " + String(millisBetter() - startMill));
#endif
    
    return true;
}

void disconnectWifiQuick()
{
    debugLog("Turning off wifi quick, all the actions took: " + String(millisBetter() - startMill));
    turnOffWifiMinimal();
    setCpuSpeed(cs);
}