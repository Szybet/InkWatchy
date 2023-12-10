#include "weather.h"

OpenWeatherOneCall OWOC;
bool isWeatherAvailable = false;

// https://github.com/JHershey69/OpenWeatherOneCall/blob/master/examples/v2%20Examples/SerialMonitorWeatherExample.ino
void syncWeather() {
    if(OPEN_WEATHER_API_KEY != "" && city_id != 0) {
        OWOC.parseWeather(OPEN_WEATHER_API_KEY, NULL, NULL, NULL, METRIC_WEATHER, CITY_ID, EXCL_C + EXCL_M + EXCL_A, 0);
    } else {
        isWeatherAvailable = false;
    }
}
