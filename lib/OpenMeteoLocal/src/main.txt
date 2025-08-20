#include <Arduino.h>
#include <WiFi.h>
#include "OpenMeteo.h"
#include <NTPClient.h>

// Replace with your own network credentials
const char *ssid = "hotspot";
const char *password = "12345678";

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting to WiFi Network ..");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

OM_HourlyForecast *forecast = NULL;

void loop()
{
    delay(1000);
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);
    timeClient.begin();
    if (timeClient.forceUpdate() == false)
    {
        Serial.println("Failed to get ntp time");
    }
    time_t epochTime = timeClient.getEpochTime();
    Serial.println("Unix time: " + String(epochTime));
    forecast = new OM_HourlyForecast;
    bool status = getHourlyForecast(forecast, 53.55073, 9.99302, epochTime);
    delay(1000);
    if (status == true)
    {

        for (int i = 0; i < OM_WEATHER_MAX_HOURS; ++i)
        {
            Serial.print("Hour " + String(i) + ": "); // Print the hour index

            // Print each value for the current hour
            Serial.print("Time: " + String(forecast->hourly_time[i]));
            Serial.print(", Temperature: " + String(forecast->temp[i]));
            Serial.print(", Humidity: " + String(forecast->humidity[i]));
            Serial.print(", Apparent Temperature: " + String(forecast->apparent_temp[i]));
            Serial.print(", Pressure: " + String(forecast->pressure[i]));
            Serial.print(", Precipitation: " + String(forecast->precipitation[i]));
            Serial.print(", Cloud Cover: " + String(forecast->cloud_cover[i]));
            Serial.print(", Visibility: " + String(forecast->visibility[i]));
            Serial.print(", Wind Speed: " + String(forecast->wind_speed[i]));
            Serial.print(", Wind Direction: " + String(forecast->wind_deg[i]));
            Serial.print(", Wind Gust: " + String(forecast->wind_gust[i]));
            Serial.print(", Weather Code: " + String(forecast->weather_code[i]));
            Serial.println(", is Day: " + String(forecast->is_day[i]));
        }

        for (int i = 0; i < OM_WEATHER_MAX_DAYS; ++i)
        {
            Serial.print("Day " + String(i) + ": "); // Print the day index

            // Print each value for the current day
            Serial.print("Time: " + String(forecast->daily_time[i]));
            Serial.print(", sunrise: " + String(forecast->sunrise[i]));
            Serial.println(", sunset: " + String(forecast->sunset[i]));
        }
    }
    else
    {
        Serial.println("Failed to get weather forecast");
    }
    delete forecast;

    delay(7500);
}
