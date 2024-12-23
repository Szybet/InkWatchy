#include "temp.h"

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

float getTemp()
{
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/temp_sensor.html
    // Performance of this is not great
    temperature_sensor_handle_t temp_handle = NULL;
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(0, 80);
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_handle));
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));
    float tsens_out;
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_handle, &tsens_out));
    ESP_ERROR_CHECK(temperature_sensor_disable(temp_handle));
    return tsens_out;
}
#endif

void screenTempFix()
{
    if (rM.fixCounts <= TEMP_MAX_SCREEN_FIXES && rM.updateCounter < FULL_DISPLAY_UPDATE_QUEUE)
    {
        rM.updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
        rM.fixCounts = rM.fixCounts + 1;
    }
}

void tempChecker()
{
    float newTemp = getTemp();
    if (newTemp > rM.previousTemp + 1.5 || newTemp < rM.previousTemp - 1.5)
    {
        if (rM.initialTemp == 0.0)
        {
            debugLog("Initial temp is: " + String(newTemp));
            rM.initialTemp = newTemp;
        }
        debugLog("Temperature changed to: " + String(newTemp));
        rM.previousTemp = newTemp;
        if (newTemp > rM.initialTemp + float(TEMP_REBOOT_LIMIT_RELATIVE))
        {
            debugLog("Temperature too high, exiting");
            assert(true == false);
        } else if (newTemp > rM.initialTemp + float(TEMP_HIGHER_LIMIT_RELATIVE))
        {
            screenTempFix();
        }
        else if (newTemp < rM.initialTemp + float(TEMP_LOWER_LIMIT_RELATIVE))
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