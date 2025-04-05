#include "i2c.h"

#if ATCHY_VER == YATCHY || ATCHY_VER == WATCHY_3 
  #if ATCHY_VER == YATCHY
    #define I2C_SDA_PIN 22
    #define I2C_SCL_PIN 23
    #define I2C_FREQ 100 // In Khz, BMA456 requires 100 not 50
  #elif ATCHY_VER == WATCHY_3
    #define I2C_SDA_PIN 12
    #define I2C_SCL_PIN 11
    #define I2C_FREQ 10 // In Khz
  #endif

bool initedI2C = false;
uint8_t i2cInitCount = 0;

bool initI2C()
{
    if (initedI2C == false)
    {
        debugLog("Starting to init I2C line");
        if (i2cInitCount > 5)
        {
            return false;
        }
        if (Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ * 1000) == false)
        {
            debugLog("Failed to begin I2C");
            i2cInitCount = i2cInitCount + 1;
            delayTask(10);
            return initI2C();
        } else {
            debugLog("Inited I2C line");
        }
        initedI2C = true;
    }
    return true;
}

void deInitI2C() {
    initedI2C = false;
    i2cInitCount = 10; // This turns it off forever in this session
    bool wireEnd = Wire.end();
    debugLog("Wire end status: " + BOOL_STR(wireEnd));
}
#endif