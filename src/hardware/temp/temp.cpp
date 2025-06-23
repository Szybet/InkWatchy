#include "temp.h"
#include "rtcMem.h"

#if TEMP_CHECKS_ENABLED
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

float getTemp()
{
    return float(temprature_sens_read() - 32) / 1.8 + float(TEMP_REAL_OFFSET);
}
#else
#include "driver/temperature_sensor.h"

temperature_sensor_handle_t temp_handle = NULL;
temperature_sensor_config_t temp_sensor_config;

float getTemp()
{
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/temp_sensor.html
    // Performance of this is not great

    if (temp_handle == NULL)
    {
        temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(TEMP_MINIMUM, TEMP_MAXIMUM);
        ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_handle));
        ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));
    }
    float tsens_out;
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_handle, &tsens_out));
    // ESP_ERROR_CHECK(temperature_sensor_disable(temp_handle));
    return tsens_out + TEMP_REAL_OFFSET;
}
#endif

void screenTempFix()
{
    #if !TEMP_ESP32 && (ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5)
    return;
    #endif

    #if !TEMP_ESP32S3 && (ATCHY_VER == WATCHY_3)
    return;
    #endif

    #if !TEMP_ESP32C6 && (ATCHY_VER == YATCHY)
    return;
    #endif
    
    if (rM.fixCounts <= TEMP_MAX_SCREEN_FIXES && rM.updateCounter < FULL_DISPLAY_UPDATE_QUEUE)
    {
        rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
        rM.fixCounts = rM.fixCounts + 1;
    }
}

void tempChecker()
{
    float newTemp = getTemp();
    // debugLog("Temp is now: " + String(newTemp));
    if (newTemp > rM.previousTemp + 1.5 || newTemp < rM.previousTemp - 1.5)
    {
        if (rM.initialTemp == 0.0)
        {
            debugLog("Initial temp is: " + String(newTemp));
            rM.initialTemp = newTemp;
        }
        debugLog("Temperature changed to: " + String(newTemp));
        rM.previousTemp = newTemp;
        if (newTemp > float(TEMP_REBOOT_LIMIT))
        {
            debugLog("Temperature too high, exiting");
            assert(true == false);
        }
        else if (newTemp > float(TEMP_HIGHER_LIMIT))
        {
            screenTempFix();
        }
        else if (newTemp < float(TEMP_LOWER_LIMIT))
        {
            screenTempFix();
        }
        else
        {
            rM.fixCounts = 0;
        }
    }
}

#endif

/*
// For my yatchy for example
room temp is: 25.6
from recorded 7.0
so the diff is
18.6
*/