#ifndef MCP23018_H
#define MCP23018_H

#include <defines/defines.h>

#if ATCHY_VER == YATCHY

#define MCP23018_ADDRESS 0x27 // https://www.chiark.greenend.org.uk/~peterb/electronics/mcp23018/addr
// gpio20 on prototype PCB but gpio6 as ADC pin fix
#define MCP_INTERRUPT_PIN 6 // Pin on esp for the interrupt
/*
#define MCP23018_SDA_PIN 22
#define MCP23018_SCL_PIN 23
#define MCP23018_I2C_FREQ 10 // In Khz
*/

#define MCP_OUTPUT 1
#define MCP_INPUT 0

/*
For interrupts, read the datasheet one more time
*/
/* Controls the direction of data I/O
1 = Pin is configured as an input.
0 = Pin is configured as an output*/
#define IODIR 0x00
/* Controls the polarity inversion of the input pins
1 = GPIO register bit will reflect the opposite logic state of the input pin.
0 = GPIO register bit will reflect the same logic state of the input pin
Why does this exist */
#define IPOL 0x02
/* General purpose I/O interrupt-on-change pins
1 = Enable GPIO input pin for interrupt-on-change event
0 = Disable GPIO input pin for interrupt-on-change event. */
#define GPINTEN 0x04
/* Sets the compare value for pins configured for interrupt-on-change from defaults.
If the associated pin level is the opposite from the register bit, an interrupt occurs. */
#define DEFVAL 0x06
/* Controls how the associated pin value is compared for interrupt-on-change.
1 = Pin value is compared against the associated bit is DEFVAL register
0 = Pin value is compared against the previous pin value. */
#define INTCON 0x08
/* EXPANDER CONFIGURATION REGISTER */
#define IOCON 0x0A
/* Controls the internal pull-up resistors on each pin (when configured as an input or output)
1 = Pull-up enabled.
0 = Pull-up disabled */
#define GPPU 0x0C
/* Reflects the interrupt condition on the port. Will reflect the change only if interrupts are
enabled (GPINTEN).
1 = Pin caused interrupt.
0 = Interrupt not pending. */
#define INTF 0x0E
/* Reflects the logic level on the port pins at the time of interrupt due to pin change.
1 = Logic-high.
0 = Logic-low. */
#define INTCAP 0x10
/* Reflects the logic level on the pin.
Reading from this register reads the port. Writing to this
register modifies the Output Latch (OLAT) register
1 = Logic-high.
0 = Logic-low. */
#define GPIO 0x12
/* Reflects the logic level on the output latch.
Not really useful, this register shows which value we want and GPIO register which value there is. Maybe detecting shorts, but not really
1 = Logic-high.
0 = Logic-low. */
#define OLAT 0x14

class mcp23018
{
public:
  mcp23018();
  void init(bool fromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason);
  buttonState manageInterrupts();
  bool manageInterruptsExit();
  void setDefaultInterrupts();
  void setDefaultInterruptsEsp();
  void setInterrupt(uint8_t pin, bool interrupt);
  void setPinMode(uint8_t pin, bool mode); // false input, true output
  void setPinState(uint8_t pin, bool state);
  void setPinPullUp(uint8_t pin, bool pull);
  bool digitalRead(uint8_t pin);
#if DEBUG
  void dumpAllRegisters();
#endif

private:
  uint16_t iodirReg;
  uint16_t gpintenReg;
  uint16_t gppuReg;
  uint16_t olatReg;

  void resetVerify();
  int expectInterruptState;

  void setBit(uint16_t &val, uint8_t bit, bool state);
  bool checkBit(uint16_t val, uint8_t bit);
  void writeRegister(uint8_t reg, uint16_t val);
  void writeSingleRegister(uint8_t reg, uint8_t val);
  uint16_t readRegister(uint8_t reg);
  uint8_t readSingleRegister(uint8_t reg);
};

extern mcp23018 gpioExpander;

#if DEBUG
String uint16ToBinaryString(uint16_t value);
String uint8ToBinaryString(uint8_t value);
#endif

#endif
#endif