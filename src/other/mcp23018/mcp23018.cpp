#include "mcp23018.h"

#if ATCHY_VER == YATCHY

RTC_DATA_ATTR mcp23018 gpioExpander;

#define EMPTY_REG 0
#define FULL_REG 0xFFFF

void manageGpioExpanderInt()
{
  interruptedButton = Unknown;
  resumeButtonTask();
}

mcp23018::mcp23018() {}

void mcp23018::init(bool fromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
  debugLog("Begin mcp23018 init");
  if (fromWakeUp == false)
  {
    resetVerify();
  }
}

void mcp23018::setDefaultInterruptsEsp()
{
  //pinMode(MCP_INTERRUPT_PIN, INPUT); // maybe no
  attachInterrupt(digitalPinToInterrupt(MCP_INTERRUPT_PIN), manageGpioExpanderInt, expectInterruptState);
}

buttonState mcp23018::manageInterrupts()
{
  initI2C();
  debugLog("Launched manageInterrupts");
  uint16_t interrupts = readRegister(INTCAP);
  debugLog("Interrupt bits: " + uint16ToBinaryString(interrupts));
  dumpAllRegisters();
  // What is going on here
  
#ifdef YATCHY_BACK_BTN
  if (checkBit(interrupts, BACK_PIN) == true)
  {
    debugLog("Gpio expander back");
    return Back;
  }
#endif

  if (checkBit(interrupts, MENU_PIN) == true)
  {
    debugLog("Gpio expander menu");
    return Menu;
  }

  if (checkBit(interrupts, DOWN_PIN) == true)
  {
    debugLog("Gpio expander down pin");
    return Down;
  }

  if (checkBit(interrupts, UP_PIN) == true)
  {
    debugLog("Gpio expander up pin");
    return Up;
  }

  debugLog("No interrupt? returning none");
  return None;
}

void mcp23018::resetVerify()
{
  initI2C(); // When this fails, we are lost
  // Init to default value
  writeRegister(0, FULL_REG);
  for (byte i = 2; i < 22; i = i + 2)
  {
    writeRegister(i, EMPTY_REG);
  }
  // isDebug(dumpAllRegisters());

  // Verify
#if DEBUG
  bool somethingWrong = false;
  if (readRegister(0) != FULL_REG)
  {
    debugLog("First register is wrong");
    somethingWrong = true;
  }
  for (uint8_t i = 2; i < 22; i = i + 2)
  {
    if (readRegister(i) != EMPTY_REG)
    {
      debugLog("Register " + String(i) + " is wrong");
      somethingWrong = true;
    }
  }
  if (somethingWrong == true)
  {
    debugLog("Something is really wrong with the expander!");
  }
#endif
  isDebug(dumpAllRegisters());
  // IOCON register
  // mirror 1
  // odr to 1 if battery voltage is above 3.0V
  // intpol to 1 if battery voltage is below 3.0V
  // intcc to 1
  /*
  uint8_t bitToHigh = 0;
  if (BatteryRead() > 3.0)
  {
    bitToHigh = 2;
    expectInterruptState = FALLING;
  }
  else
  {
    bitToHigh = 1;
    expectInterruptState = RISING;
  }
  */
  uint8_t bitToHigh = 0;
  bitToHigh = 2;
  expectInterruptState = FALLING; // FALLING
  if (BatteryRead() < 3.0)
  {
    debugLog("Interrupt is already low?");
  }

  uint8_t iocon = 0b01000001 | (1 << bitToHigh);
  debugLog("Final iocon is: " + uint8ToBinaryString(iocon));

  writeSingleRegister(IOCON, iocon);
  writeSingleRegister(IOCON + 1, iocon);

  iodirReg = FULL_REG;
  gpintenReg = EMPTY_REG;
  gppuReg = EMPTY_REG;
  olatReg = EMPTY_REG;

  // isDebug(dumpAllRegisters());

  // Set screen cs to low
  setPinMode(YATCHY_DISPLAY_CS, MCP_OUTPUT);
  // setPinPullUp(YATCHY_DISPLAY_CS, false); // Not needed, it's false at default
  setPinState(YATCHY_DISPLAY_CS, LOW);

  setDefaultInterrupts();
  isDebug(dumpAllRegisters());
}

bool mcp23018::digitalRead(uint8_t pin)
{
  initI2C();
  // Manage YATCHY_BACK_BTN not existing here
  // Also consider using interrupts
  return checkBit(readRegister(GPIO), pin);
}

void mcp23018::setDefaultInterrupts()
{
#ifdef YATCHY_BACK_BTN
  setInterrupt(BACK_PIN, true);
#endif
  setInterrupt(MENU_PIN, true);
  setInterrupt(DOWN_PIN, true);
  setInterrupt(UP_PIN, true);
}

void mcp23018::setInterrupt(uint8_t pin, bool interrupt)
{
  setBit(gpintenReg, pin, interrupt);
  // debugLog("gpintenReg: " + uint16ToBinaryString(gpintenReg));
  writeRegister(GPINTEN, gpintenReg);
}

// Use MCP_OUTPUT and MCP_INPUT here
void mcp23018::setPinMode(uint8_t pin, bool mode)
{
  // NOT is here
  setBit(iodirReg, pin, !mode);
  // debugLog("iodirReg: " + uint16ToBinaryString(iodirReg));
  writeRegister(IODIR, iodirReg);
}

void mcp23018::setPinState(uint8_t pin, bool state)
{
  setBit(olatReg, pin, state);
  // debugLog("olatReg: " + uint16ToBinaryString(olatReg));
  writeRegister(OLAT, olatReg);
}

void mcp23018::setPinPullUp(uint8_t pin, bool pull)
{
  setBit(gppuReg, pin, pull);
  // debugLog("gppuReg: " + uint16ToBinaryString(gppuReg));
  writeRegister(GPPU, gppuReg);
}

void mcp23018::setBit(uint16_t &val, uint8_t bit, bool state)
{
  if (state)
  {
    val |= (1 << bit);
  }
  else
  {
    val &= ~(1 << bit);
  }
}

bool mcp23018::checkBit(uint16_t val, uint8_t bit)
{
  return (val & (1 << bit)) != 0;
}

void mcp23018::writeSingleRegister(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void mcp23018::writeRegister(uint8_t reg, uint16_t val)
{
  // debugLog("Writing value: " + uint16ToBinaryString(val));
  uint8_t byte0 = val & 0xFF;
  uint8_t byte1 = (val >> 8) & 0xFF;
  // debugLog("Bytes to write, bare: " + String(byte0) + " " + String(byte1));
  /*
  uint8_t buffer[2];
  buffer[0] = byte0;
  buffer[1] = byte1;
  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg);
  Wire.write(buffer, 2);
  Wire.endTransmission();
  */
  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg);
  Wire.write(byte0);
  Wire.write(byte1);
  Wire.endTransmission();
}

uint16_t mcp23018::readRegister(uint8_t reg)
{
  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MCP23018_ADDRESS, 2);
  Wire.available();
  byte data1 = Wire.read();
  byte data2 = Wire.read();
#if DEBUG
  if (Wire.available())
  {
    debugLog("There are more bytes available?");
  }
#endif

  // debugLog("Received pure bytes: " + String(data1) + " " + String(data2));

  uint16_t combined = (static_cast<uint16_t>(data1)) | data2 << 8;
  // debugLog("Received data: " + uint16ToBinaryString(combined) + " in register: " + String(reg));
  return combined;
}

uint8_t mcp23018::readSingleRegister(uint8_t reg)
{
  Wire.beginTransmission(MCP23018_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MCP23018_ADDRESS, 1);
  Wire.available();
  uint8_t readed = Wire.read();
  // debugLog("Readed single byte: " + uint8ToBinaryString(readed));
  return readed;
}

#if DEBUG
void mcp23018::dumpAllRegisters()
{
#if true == true
  for (byte i = 0; i < 22; i = i + 2)
  {
    debugLog("Register: " + String(i) + " is: " + uint16ToBinaryString(readRegister(i)));
  }
#else
  for (byte i = 0; i < 22; i++)
  {
    debugLog("Register: " + String(i) + " is: " + uint8ToBinaryString(readSingleRegister(i)));
  }
#endif
}

String uint16ToBinaryString(uint16_t value)
{
  String binaryString = "";
  for (int i = 15; i >= 0; i--)
  {
    binaryString += (value & (1 << i)) ? '1' : '0';
  }
  return binaryString;
}

String uint8ToBinaryString(uint8_t value)
{
  String binaryString = "";
  for (int i = 7; i >= 0; i--)
  {
    binaryString += (value & (1 << i)) ? '1' : '0';
  }
  return binaryString;
}

#endif
#endif
