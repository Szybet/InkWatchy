#include "defines.h"

// https://github.com/espressif/arduino-esp32/blob/337058ac94e7e3df11d273a93e88d1ea605e6f5f/cores/esp32/main.cpp#L105
// Why is that? well anyway this task set's it and exits
TaskHandle_t priorityLoopHandle;
void priorityLoopSet(void *parameter)
{
  while (true)
  {
    delayTask(350);
    TaskHandle_t loopTaskHandle = xTaskGetHandle("loopTask");
    if (loopTaskHandle != NULL)
    {
      vTaskPrioritySet(loopTaskHandle, MAIN_LOOP_PRIORITY);
      // debugLog("Set loop task priority, exiting...");
      vTaskDelete(NULL);
    }
  }
}

void setup()
{
#if DEBUG
  initLog();
#endif

  initHardware();
  // debugLog("Starting millis: " + String(millisBetter()));

#if DEBUG
  showInitLogs();
#endif

  initManager();

  if (bootStatus.reason != rtc)
  {
    // I trust myself enough now to not need watchdog task running all the time
    initWatchdogTask();

    turnOnButtons();

    xTaskCreate(
        priorityLoopSet,
        "priorityLoop",
        1000,
        NULL,
        20,
        &priorityLoopHandle);
  }

  // resetSleepDelay();
  // HOLY SHIT I WAS DOING A SHORT but now it doesnt work?
  gpioExpander.setPinState(MCP_5V, true);
  gpioExpander.setPinMode(MCP_5V, MCP_OUTPUT);
  setCpuSpeed(maxSpeed);
}

int fuck = 0;
void loop()
{
  // gpioExpander.setPinState(MCP_5V, false);
  // gpioExpander.setPinState(MCP_5V, true);
  bool fiveVolt = gpioExpander.digitalRead(MCP_5V);
  display.setCursor(100, 100);
  display.fillRect(0, 0, 200, 200, GxEPD_WHITE);
  display.print(String(fiveVolt));
  watchdogPing();
  fuck = fuck + 1;
  display.setCursor(100, 150);
  display.print(String(fuck));
  display.display(PARTIAL_UPDATE);
  if (fuck > 30)
  {
    gpioExpander.setPinMode(MCP_5V, MCP_INPUT);
    goSleep();
  }
  delayTask(1000);
}
