#pragma once

// The priority (order) here matters a bit
STATIC_WIFI_CRED wifi_credential1 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential2 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential3 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential4 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential5 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential6 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential7 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential8 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential9 = {.ssid = "", .password = ""};
STATIC_WIFI_CRED wifi_credential10 = {.ssid = "hotspot", .password = "12345678"};

// Wifi Quick settings
#define WQ_SSID ""
#define WQ_PASS ""
#define WQ_BSSID "" // Mac, splitted by ":"
// Yes, this is the format, without ""
#define WQ_STAT_IP 0, 0, 0, 0
#define WQ_GATEWAY 0, 0, 0, 0
#define WQ_SUBNET 0, 0, 0, 0
#define WQ_CONN_TO 0, 0, 0, 0 // Ip address to connect to
#define WQ_PORT_TO 54321      // Port to connect to

// Get those values here: https://open-meteo.com/en/docs/geocoding-api
// For hamburg, germany example values:
#define WEATHER_LATIT ""   // 53.55073
#define WEATHER_LONGTIT "" // 9.99302

#define COIN_LIB_API_KEY "" // It will work without it, just with less informations

#define VAULT_PASSWORD "" // This must be a numerical PIN, don't start it with 0

// If you want just a path, implement it yourself, look up the script and the program arguments
// Link to your ICS file to download it
#define CALENDAR_URL "" // https://MyIcsFileSomewhere.toDownload

