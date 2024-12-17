#include "acc.h"
#include "rtcMem.h"

#if AXC_ENABLED

uint16_t readRegisterBMA(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
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
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data, len);
    return (0 != Wire.endTransmission());
}

void initAxc()
{
    debugLog("initAxc Launched");
    if (rM.initedAxc == false)
    {
        uint8_t Type = rM.SRTC.getType(); // TODO: fix this
        if (rM.SBMA.begin(readRegisterBMA, writeRegisterBMA, delay, Type) == false)
        {
            debugLog("Failed to init bma");
            return;
        }

        if (!rM.SBMA.defaultConfig())
        {
            debugLog("Failed to init bma - default config");
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
            rM.SBMA.enableFeature(BMA423_STEP_CNTR, true);
            rM.SBMA.resetStepCounter();
        } else {
            if(rM.stepDay != timeRTCLocal.Day) {
                rM.stepDay = timeRTCLocal.Day;
                rM.SBMA.resetStepCounter();
            } else {
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