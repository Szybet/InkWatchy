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

RTC_DATA_ATTR uint8_t fixCounts = 0;
void screenTempFix()
{
    if (fixCounts <= TEMP_MAX_SCREEN_FIXES && updateCounter < FULL_DISPLAY_UPDATE_QUEUE)
    {
        updateCounter = FULL_DISPLAY_UPDATE_QUEUE;
        fixCounts = fixCounts + 1;
    }
}

RTC_DATA_ATTR float initialTemp = 0.0;
RTC_DATA_ATTR float previousTemp = 0.0;
void tempChecker()
{
    float newTemp = getTemp();
    if (newTemp > previousTemp + 1.5 || newTemp < previousTemp - 1.5)
    {
        if (initialTemp == 0.0)
        {
            debugLog("Initial temp is: " + String(newTemp));
            initialTemp = newTemp;
        }
        debugLog("Temperature changed to: " + String(newTemp));
        previousTemp = newTemp;
        if (newTemp > initialTemp + float(TEMP_REBOOT_LIMIT_RELATIVE))
        {
            debugLog("Temperature too high, exiting");
            assert(true == false);
        } else if (newTemp > initialTemp + float(TEMP_HIGHER_LIMIT_RELATIVE))
        {
            screenTempFix();
        }
        else if (newTemp < initialTemp + float(TEMP_LOWER_LIMIT_RELATIVE))
        {
            screenTempFix();
        }
        else
        {
            fixCounts = 0;
        }
    }
}

#endif