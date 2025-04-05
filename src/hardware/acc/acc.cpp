#include "acc.h"
#include "rtcMem.h"

#if ACC_ENABLED

uint16_t readRegisterBMA(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
#if ATCHY_VER == YATCHY || ATCHY_VER == WATCHY_3
    if (initI2C() == false)
    {
        return 0;
    }
#endif
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)address, (uint8_t)len);
    uint8_t i = 0;
    while (Wire.available())
    {
        data[i++] = Wire.read();
    }
    return 0;
}

uint16_t writeRegisterBMA(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
#if ATCHY_VER == YATCHY || ATCHY_VER == WATCHY_3
    if (initI2C() == false)
    {
        return 0;
    }
#endif
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data, len);
    return (0 != Wire.endTransmission());
}

void lookForFalse(bool newBool, bool *oldValue)
{
    if (*oldValue == false)
    {
        return;
    }
    if (newBool == false)
    {
        *oldValue = false;
        return;
    }
}

// Credits to TinyWatchy
bool accConfig()
{
    bool status = true;
    // Enabling default BMA config
    lookForFalse(rM.SBMA.defaultConfig(true), &status);
    if(status == false) {
        debugLog("defaultConfig failed");
        return status;
    }
    lookForFalse(rM.SBMA.enableAccel(), &status);
    if(status == false) {
        debugLog("enableAccel failed");
        return status;
    }

    if(BMA_VERSION == 423) {
        lookForFalse(rM.SBMA.enableFeature(BMA423_STEP_CNTR, true), &status);
        if(status == false) {
            debugLog("enableFeature(BMA423_STEP_CNTR failed");
            return status;
        }
    } else if(BMA_VERSION == 456) {
        lookForFalse(rM.SBMA.enableFeature(BMA456_STEP_CNTR, true), &status);
        if(status == false) {
            debugLog("enableFeature(BMA456_STEP_CNTR failed");
            return status;
        }
    }

    /*
    // We should not need this, Watchy v3 users maybe
    lookForFalse(rM.SBMA.enableFeature(BMA423_STEP_CNTR_INT, true), &status);
    if(status == false) {
        debugLog("enableFeature(BMA423_STEP_CNTR_INT failed");
        return status;
    }
    */

    return status;
}

void initAxc()
{
    debugLog("initAxc Launched");
    if (rM.initedAxc == false)
    {
        uint8_t type;
#if ATCHY_VER == WATCHY_1
        type = 1;
#elif ATCHY_VER == WATCHY_1_5
        type = 2;
#elif ATCHY_VER == WATCHY_2
        type = 2;
#elif ATCHY_VER == WATCHY_3
        type = 3;
#elif ATCHY_VER == YATCHY
        type = 4;
#endif

        debugLog("Acc type is: " + String(type));
        if(rM.SBMA.__init == false) {
            if (rM.SBMA.begin(readRegisterBMA, writeRegisterBMA, vTaskDelay, type, BMA4_I2C_ADDR_PRIMARY, false, -1, -1, BMA_VERSION) == false)
            {
                debugLog("Failed to init bma");
                return;
            }    
        }

        if (!accConfig())
        {
            debugLog("Failed to init bma - config");
            return;
        }
        rM.initedAxc = true;
    }
    else
    {
        debugLog("Axc is already inited");
    }
}

// All in one function to get steps, it managed everything
// TODO: after changing watchface that doesn't use steps, the acc is still turned on with this feature while its not used
uint16_t getSteps()
{
    uint16_t steps = 0;
    if (rM.initedAxc == true)
    {
        if (rM.stepsInited == false)
        {
            rM.stepsInited = true;
            rM.SBMA.resetStepCounter();
        }
        else
        {
            if (rM.stepDay != timeRTCLocal.Day)
            {
                rM.stepDay = timeRTCLocal.Day;
                rM.SBMA.resetStepCounter();
            }
            else
            {
                steps = (uint16_t)rM.SBMA.getCounter();
            }
        }
    }
    else
    {
        initAxc();
        return getSteps();
    }
    debugLog("Returning steps: " + String(steps));
    return steps;
}
#else

uint16_t getSteps()
{
    debugLog("AXC is turned off. This is a fallback function");
    return 1234;
}

#endif