#include "mcp23018.h"

#if ATCHY_VER == YATCHY

mcp23018::mcp23018(bool fromWakeUp) {
    debugLog("Begin mcp23018 init");
    initI2C(); // When this fails, we are lost
    if(fromWakeUp == false) {
        if(resetVerify() == false) {
            debugLog("Failed to reset and verify the gpio expander");
        }
    }
}

bool mcp23018::resetVerify() {

    // Verify
    for(byte i = 0; i < 24; i++) {
        readRegister(i);
    }

    return false;
}

void mcp23018::writeRegister(uint8_t reg, uint16_t val) {

}

uint16_t mcp23018::readRegister(uint8_t reg) {

  Wire.requestFrom(MCP23018_ADDRESS, 1); 

  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg); 
  Wire.endTransmission(); 

  if (Wire.available()) {
    byte data = Wire.read(); 
    Serial.println(data);
  }

  delay(1000);

  return 0;
}

#endif