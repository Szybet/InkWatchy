#include "acc.h"
#include "rtcMem.h"

#if ACC_ENABLED

#define ACC_MAX_TRIES 8

#if BMAM_4
uint16_t readRegisterBMA4(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
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

uint16_t writeRegisterBMA4(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
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

#elif BMAM_5
int8_t readRegisterBMA5(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
#if ATCHY_VER == YATCHY
    if (initI2C() == false)
    {
        return -1; // Return non-zero on initialization failure
    }
#endif

    if (intf_ptr == NULL)
    {
        return -1; // Handle NULL pointer error
    }

    uint8_t address = *(uint8_t *)intf_ptr; // Extract device address from intf_ptr

    Wire.beginTransmission(address);
    Wire.write(reg_addr);
    int8_t endResult = Wire.endTransmission(); // Complete transmission

    if (endResult != 0)
    {
        return endResult; // Return I2C transmission error code
    }

    // Request 'length' bytes from the device
    uint8_t received = Wire.requestFrom(address, (uint8_t)length);
    if (received != length)
    {
        return -2; // Not all bytes received
    }

    // Read received bytes into data buffer
    for (uint32_t i = 0; i < length; i++)
    {
        if (!Wire.available())
        {
            return -3; // Data not available when expected
        }
        reg_data[i] = Wire.read();
    }

    return 0; // Success
}

int8_t writeRegisterBMA5(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length,
                         void *intf_ptr)
{
#if ATCHY_VER == YATCHY
    if (initI2C() == false)
    {
        return -1; // Return non-zero on initialization failure
    }
#endif

    if (intf_ptr == NULL)
    {
        return -1; // Handle NULL pointer error
    }

    uint8_t address = *(uint8_t *)intf_ptr; // Extract device address from intf_ptr

    Wire.beginTransmission(address);
    Wire.write(reg_addr);                   // Send register address
    Wire.write(reg_data, length);           // Send data bytes
    int8_t result = Wire.endTransmission(); // Complete transmission and get status

    return result; // Return 0 on success, non-zero on failure
}
#endif

// Credits to TinyWatchy
bool accConfig()
{
    bool status = true;
    // Enabling default BMA config
    if (rM.SBMA.defaultConfig(true) == false)
    {
        debugLog("defaultConfig failed");
        return false;
    }

    // This is not needed for steps, idk for else
    // lookForFalse(rM.SBMA.enableAccel(), &status);

    // if (status == false)
    // {
    //     debugLog("enableAccel failed");
    //     return false;
    // }

    if (rM.SBMA.enableStepCount() == false) {
        debugLog("enableStepCount failed");
        return false;
    }

    /*
    // We should not need this for pure step counting, Watchy v3 users maybe
    lookForFalse(rM.SBMA.enableFeature(BMA423_STEP_CNTR_INT, true), &status);
    if(status == false) {
        debugLog("enableFeature(BMA423_STEP_CNTR_INT failed");
        return status;
    }
    */

    return true;
}

void initAcc()
{
    debugLog("initAcc Launched");
    if (rM.initedAcc == false)
    {
        rM.initAccTries = rM.initAccTries + 1;
        if (rM.initAccTries > ACC_MAX_TRIES)
        {
            debugLog("Acc init try limit");
            return;
        }
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

        debugLog("Acc watchy type is: " + String(type));
        debugLog("BMA version: " + String(BMA_VERSION));
#if BMAM_4
        if (rM.SBMA.begin4(type, BMA4_I2C_ADDR_PRIMARY, BMA_VERSION, readRegisterBMA4, writeRegisterBMA4) == false)
        {
            debugLog("Failed to init bma");
            return;
        }
#elif BMAM_5
        if (rM.SBMA.begin5(type, BMA4_I2C_ADDR_PRIMARY, BMA_VERSION, readRegisterBMA5, writeRegisterBMA5) == false)
        {
            debugLog("Failed to init bma");
            return;
        }
#endif

        if (accConfig() == false)
        {
            debugLog("Failed to init bma - config");
            return;
        }

// Frezes acc data somehow
#if 0
        if(rM.SBMA.selfTest() == false) {
            debugLog("Self test failed");
            return;
        }
#endif

        rM.initedAcc = true;
    }
    else
    {
        debugLog("Axc is already inited");
    }
}

// All in one function to get steps, it manages everything
// TODO: after changing watchface that doesn't use steps, the acc is still turned on with this feature while its not used
uint16_t getSteps()
{    
    uint16_t steps = 0;
    if (rM.initedAcc == true)
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
        if (rM.initAccTries > ACC_MAX_TRIES)
        {
            debugLog("Too many init tries");
            return 8;
        }
        initAcc();
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