#include "acc.h"
#include "rtcMem.h"

#if AXC_ENABLED

uint16_t readRegisterBMA(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
#if ATCHY_VER == YATCHY && 1 == 0
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
#if ATCHY_VER == YATCHY && 1 == 0
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
    // Setting BMA4_OUTPUT_DATA_RATE_0_78HZ gives good power consumption but probably sucks
    // rM.SBMA.shutDown() shuts down power consumption, duh
    Acfg cfg = {
        .odr = BMA4_OUTPUT_DATA_RATE_50HZ,
        .bandwidth = BMA4_ACCEL_NORMAL_AVG4,
        .perf_mode = BMA4_CIC_AVG_MODE,
        .range = BMA4_ACCEL_RANGE_2G,
    };
    // BMA4_OUTPUT_DATA_RATE_100HZ - 72 steps
    // BMA4_OUTPUT_DATA_RATE_50HZ - 62 steps - best I ques, but other acc functions don't work well here, so if used, use 100HZ instead
    // BMA4_OUTPUT_DATA_RATE_25HZ - 28 steps

    lookForFalse(rM.SBMA.setAccelConfig(cfg), &status);
    rM.SBMA.wakeUp();
    lookForFalse(rM.SBMA.enableAccel(), &status);

    struct bma4_int_pin_config config = {
        .edge_ctrl = BMA4_EDGE_TRIGGER,
        .lvl = BMA4_ACTIVE_HIGH,
        .od = BMA4_PUSH_PULL,
        .output_en = BMA4_OUTPUT_ENABLE,
        .input_en = BMA4_INPUT_DISABLE,
    };

    // lookForFalse(rM.SBMA.setINTPinConfig(config, BMA4_INTR1_MAP), status);

    struct bma423_axes_remap remap_data = {
        .x_axis = 1,
        .x_axis_sign = 0,
        .y_axis = 0,
        .y_axis_sign = 0,
        .z_axis = 2,
        .z_axis_sign = 1,
    };

    lookForFalse(rM.SBMA.setRemapAxes(&remap_data), &status);
    lookForFalse(rM.SBMA.enableFeature(BMA423_STEP_CNTR, true), &status);

    // lookForFalse(rM.SBMA.enableFeature(BMA423_WAKEUP, true), status);

    // lookForFalse(rM.SBMA.resetStepCounter(), &status);

    // lookForFalse(rM.SBMA.enableWakeupInterrupt(), status);

    return status;
}

void initAxc()
{
    debugLog("initAxc Launched");
    if (rM.initedAxc == false)
    {
        uint8_t Type = rM.SRTC.getType(); // TODO: fix this
        if(Type == 0) {
            debugLog("Type SRTC is invalid, fix this, defaulting to 2");
            Type = 2;
        }
        if (rM.SBMA.begin(readRegisterBMA, writeRegisterBMA, delay, Type) == false)
        {
            debugLog("Failed to init bma");
            return;
        }

        if (!accConfig())
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