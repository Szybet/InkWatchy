#include "axc.h"

RTC_DATA_ATTR StableBMA SBMA;

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
    debugLog("Launched");
    uint8_t Type = SRTC.getType();
    if (SBMA.begin(readRegisterBMA, writeRegisterBMA, delay, Type) == false)
    {
        debugLog("Failed to init bma");
        return;
    }

    if (!SBMA.defaultConfig())
    {
        debugLog("Failed to init bma - default config");
    }

    // SBMA.enableFeature(BMA423_STEP_CNTR, true);
}

void deInitAxc()
{
    debugLog("Launched");
    SBMA.shutDown();
}
