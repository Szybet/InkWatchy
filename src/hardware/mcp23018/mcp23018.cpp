#include "mcp23018.h"

#if ATCHY_VER == YATCHY

RTC_DATA_ATTR mcp23018 gpioExpander;

#define EMPTY_REG 0
#define FULL_REG 0xFFFF

bool ignoreInterrupt = false;
void manageGpioExpanderInt()
{
#if DEBUG
  // Serial.println("mcpI"); // Potential crash
#endif
  if (ignoreInterrupt == false)
  {
#if DEBUG
    // Serial.println("mcpD"); // Potential crash
#endif
    ignoreInterrupt = true;
    interruptedButton = Unknown;
    resumeButtonTask();
  }
}

mcp23018::mcp23018() {}

bool mcp23018::simplerInit()
{
  if (inited == false && initOngoing == false)
  {
    mcpMutex.lock();
    if (inited == true)
    {
      mcpMutex.unlock();
      return true;
    }
    if (initCount > 5)
    {
      mcpMutex.unlock();
      return false;
    }
    initOngoing = true;
    if (resetVerify() == false)
    {
      initOngoing = false;
      debugLog("Failed to reset-verify the expander");
      initCount = initCount + 1;
      delayTask(10);
      mcpMutex.unlock();
      return simplerInit();
    }
    mcpMutex.unlock();
    initOngoing = false;
    inited = true;
  }
  return true;
}

void mcp23018::setDefaultInterruptsEsp()
{
// This is not needed here?
// if (simplerInit() == false)
//{
// return;
//}
// pinMode(MCP_INTERRUPT_PIN, INPUT); // maybe no
#if MCP_GPIO_EXPANDER_DISABLE_INTERRUPTS == false || DEBUG == false
  debugLog("Attaching gpio expander interrupt pin");
  attachInterrupt(digitalPinToInterrupt(MCP_INTERRUPT_PIN), manageGpioExpanderInt, FALLING);
#else
  debugLog("Not attaching mcp23018 interrupt");
#endif
}

buttonState mcp23018::manageInterrupts()
{
  if (simplerInit() == false)
  {
    return Unknown;
  }
  debugLog("Launched manageInterrupts");
  // For an unknown reason to me, it doesn't work if this is not called here:
  BatteryRead();
  // Maybe it clears the interrupt or something, idk, not worth my time
  
  debugLog("BatteryRead1: " + String(BatteryRead()));
  // First read the cause
  uint16_t gpio_cause = readRegister(INTF);
  // Then we read the interrupts
  uint16_t gpio_ints = readRegister(INTCAP);
  // We disable all interrupts, we don't want new ones now
  // writeRegister(GPINTEN, EMPTY_REG); // or not OR NOT we can't in fact do that
  debugLog("Interrupt bits: " + uint16ToBinaryString(gpio_ints));
  debugLog("Interrupt cause: " + uint16ToBinaryString(gpio_cause));
  // dumpAllRegisters();
  //  What is going on here

  debugLog("BatteryRead2: " + String(BatteryRead()));
  buttonState selectedbtn = None;
  // I want a break call here
  while (true)
  {
    if (checkBit(gpio_cause, MCP_STAT_IN) == true || checkBit(gpio_cause, MCP_5V) == true)
    {
      debugLog("Stat pin or 5V detected");
      isChargingCheck();
      loopBattery();
      break;
    }

#ifdef YATCHY_BACK_BTN
    if (checkBit(gpio_ints, BACK_PIN) == false)
    {
      debugLog("Gpio expander back");
      selectedbtn = Back;
      break;
    }
#endif

    if (checkBit(gpio_ints, MENU_PIN) == false)
    {
      debugLog("Gpio expander menu");
      selectedbtn = Menu;
      break;
    }

    if (checkBit(gpio_ints, DOWN_PIN) == false)
    {
      debugLog("Gpio expander down pin");
      selectedbtn = Down;
      break;
    }

    if (checkBit(gpio_ints, UP_PIN) == false)
    {
      debugLog("Gpio expander up pin");
      selectedbtn = Up;
      break;
    }

    break;
  }

  return selectedbtn;
}

bool mcp23018::manageInterruptsExit()
{
#if MCP_GPIO_EXPANDER_DISABLE_INTERRUPTS == true && DEBUG == true
  return true;
#endif
  if (simplerInit() == false)
  {
    return true; // True here because of infinite loop
  }
  debugLog("Restoring interrupts");
  if (BatteryRead() < 3.0)
  {
    debugLog("Voltage below 3.0, Clearing interrupt: " + String(BatteryRead()));
    // We need to read the register one more time because it clears only when the interrupt condition clears eg the button is not clicked
    for(uint8_t i = 0; i < 10; i++ ) {
      readRegister(INTF);
      readRegister(INTCAP);
      BatteryRead();
      delayTask(10); // Give it time to restore the voltage
    }
    debugLog("Voltage after clearing it: " + String(BatteryRead()));
    if (BatteryRead() < 3.0)
    {
      debugLog("Voltage still low after reading register, running again...");
      ignoreInterrupt = false;
      manageGpioExpanderInt();
      return false;
    }
  }
  debugLog("Exiting the interrupt thing");
  ignoreInterrupt = false;
  return true;
}

bool mcp23018::resetVerify()
{
#if MCP_GPIO_EXPANDER_DISABLE == true && DEBUG == true
  return false;
#endif
  // When this fails, we are lost
  if (initI2C() == false)
  {
    return false;
  }

  if (bootStatus.fromWakeup == true)
  {
    debugLog("mcp23018 from wakeup");
    setDefaultInterrupts();
    return true;
  }

  // To clear if it exists...
  readRegister(INTCAP);

  // Init to default value
  writeRegister(0, FULL_REG);
  for (byte i = 2; i < 22; i = i + 2)
  {
    writeRegister(i, EMPTY_REG);
  }
  // isDebug(dumpAllRegisters());

  // Clear it again, to be sure...
  delayTask(10);
  readRegister(INTCAP);

  // Verify
  bool somethingWrong = false;
  if (readRegister(0) != FULL_REG)
  {
    debugLog("First register is wrong");
    somethingWrong = true;
  }
  for (uint8_t i = 2; i < 22; i = i + 2)
  {
    // 16 is weird, it persist but the voltage is what really indicates an interrupt
    if (i == 18 || i == 16)
    {
      continue;
    }
    if (readRegister(i) != EMPTY_REG)
    {
      debugLog("Register " + String(i) + " is wrong");
      somethingWrong = true;
    }
  }
#if DEBUG == false
  if (BatteryRead() < 3.0)
  {
    debugLog("Battery is low, so interrupt too?");
    somethingWrong = true;
  }
#else
  debugLog("Not checking interrupt low for mcp23018 because we are in debug mode");
#endif
  if (somethingWrong == true)
  {
    debugLog("Something is really wrong with the expander!");
    isDebug(dumpAllRegisters());
    return false;
  }

  // IOCON register
  // mirror 1
  // intpol to 1 if battery voltage is below 3.0V - Not really anymore, usb charger fault
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
  uint8_t bitToHigh = 2;
  #if DEBUG
  if (BatteryRead() < 3.0)
  {
    debugLog("Interrupt is already low?");
  }
  #endif

  uint8_t iocon = 0b01000001 | (1 << bitToHigh);
  debugLog("Final iocon is: " + uint8ToBinaryString(iocon));

  writeSingleRegister(IOCON, iocon);
  writeSingleRegister(IOCON + 1, iocon);

// For testing pure power consumption
#if DEBUG && true == false
  debugLog("Dumping registers after verify and iocon apply, and exiting");
  isDebug(dumpAllRegisters());
  return true;
#endif

  iodirReg = FULL_REG;
  gpintenReg = EMPTY_REG;
  gppuReg = EMPTY_REG;
  olatReg = EMPTY_REG;

  intconReg = EMPTY_REG;
  defvalReg = EMPTY_REG;

  // Setting to output reduces power consumption
  // But first we need not to do a short...
  // But after setting all registers to null like above...
  for(int i = 0; i < 16; i++) {
    setPinState(i, true);
    setPinMode(i, MCP_OUTPUT);
  }

  // isDebug(dumpAllRegisters());

  // Set pins to inputs as they are outputs now
  setPinMode(MCP_5V, MCP_INPUT);
  #ifdef YATCHY_BACK_BTN
  setPinMode(BACK_PIN, MCP_INPUT);
  #endif
  setPinMode(MENU_PIN, MCP_INPUT);
  setPinMode(DOWN_PIN, MCP_INPUT);
  setPinMode(UP_PIN, MCP_INPUT);

#if RGB_DIODE
  // This does the setPinState(RGB_DIODE_PIN, true);
  setRgb(IwNone);
  // setPinMode(RGB_DIODE_RED_PIN, MCP_OUTPUT);
  // setPinMode(RGB_DIODE_GREEN_PIN, MCP_OUTPUT);
  // setPinMode(RGB_DIODE_BLUE_PIN, MCP_OUTPUT);
#endif

  setPinState(MCP_STAT_OUT, true);
  setPinMode(MCP_STAT_OUT, MCP_OUTPUT);

  setDefaultInterrupts();
  isDebug(dumpAllRegisters());

  return true;
}

void mcp23018::deInit()
{
  if (initI2C() == false)
  {
    return;
  }
  // gpioExpander.setPinState(MCP_STAT_OUT, true); // test
#if MCP_GPIO_EXPANDER_DISABLE_INTERRUPTS == false
  setInterrupt(MCP_STAT_IN, false);
  setPinMode(MCP_STAT_IN, MCP_OUTPUT);
#endif
  // dumpAllRegisters();
}

bool mcp23018::digitalRead(uint8_t pin)
{
  if (simplerInit() == false)
  {
    return false;
  }
  // Manage YATCHY_BACK_BTN not existing here
  // Also consider using interrupts
  return checkBit(readRegister(GPIO), pin);
}

void mcp23018::setDefaultInterrupts()
{
#if !MCP_GPIO_EXPANDER_DISABLE_INTERRUPTS
  if (bootStatus.fromWakeup == false)
  {
    // Buttons
#ifdef YATCHY_BACK_BTN
    setInterruptCause(BACK_PIN, true, false);
    setPinPullUp(BACK_PIN, true);
    setInterrupt(BACK_PIN, true);
#endif
    setInterruptCause(MENU_PIN, true, false);
    setPinPullUp(MENU_PIN, true);
    setInterrupt(MENU_PIN, true);

    setInterruptCause(DOWN_PIN, true, false);
    setPinPullUp(DOWN_PIN, true);
    setInterrupt(DOWN_PIN, true);

    setInterruptCause(UP_PIN, true, false);
    setPinPullUp(UP_PIN, true);
    setInterrupt(UP_PIN, true);

    setInterrupt(MCP_5V, true);
  }

  // This was disabled in sleep, now we enable it
  setPinState(MCP_STAT_IN, MCP_INPUT);
  setInterrupt(MCP_STAT_IN, true);
#endif
}

void mcp23018::setInterrupt(uint8_t pin, bool interrupt)
{
  if (simplerInit() == false)
  {
    return;
  }
  setBit(gpintenReg, pin, interrupt);
  // debugLog("gpintenReg: " + uint16ToBinaryString(gpintenReg));
  writeRegister(GPINTEN, gpintenReg);
}

void mcp23018::setInterruptCause(uint8_t pin, bool enableCause, bool causeState)
{
  if (simplerInit() == false)
  {
    return;
  }
  // enableCause writes the bit to INTCON
  // causeState writes the bit to DEFVAL, but it's a NOT statement because if we want the cause to be true, we need to write the opposite which is false
  setBit(intconReg, pin, enableCause);
  setBit(defvalReg, pin, !causeState);

  writeRegister(INTCON, intconReg);
  writeRegister(DEFVAL, defvalReg);
}

// Use MCP_OUTPUT and MCP_INPUT here
void mcp23018::setPinMode(uint8_t pin, bool mode)
{
  if (simplerInit() == false)
  {
    return;
  }
  // NOT is here
  setBit(iodirReg, pin, !mode);
  // debugLog("iodirReg: " + uint16ToBinaryString(iodirReg));
  writeRegister(IODIR, iodirReg);
}

void mcp23018::setPinState(uint8_t pin, bool state)
{
  if (simplerInit() == false)
  {
    return;
  }
  setBit(olatReg, pin, state);
  // debugLog("olatReg: " + uint16ToBinaryString(olatReg));
  writeRegister(OLAT, olatReg);
}

void mcp23018::setPinPullUp(uint8_t pin, bool pull)
{
  if (simplerInit() == false)
  {
    return;
  }
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
  if (simplerInit() == false)
  {
    return;
  }
  for (byte i = 0; i < 22; i = i + 2)
  {
    debugLog("Register: " + decimalToHexString(i) + " is: " + uint16ToBinaryString(readRegister(i)));
  }
#else
  for (byte i = 0; i < 22; i++)
  {
    debugLog("Register: " + decimalToHexString(i) + " is: " + uint8ToBinaryString(readSingleRegister(i)));
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

String decimalToHexString(int decimal)
{
  char hexString[10];
  sprintf(hexString, "%X", decimal);
  String str = String(hexString);
  if (str.length() < 2)
  {
    str = "0" + str;
  }
  return "0x" + str;
}

#endif
#endif
