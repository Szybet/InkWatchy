#include "defines/defines.h"

// https://github.com/espressif/arduino-esp32/blob/337058ac94e7e3df11d273a93e88d1ea605e6f5f/cores/esp32/main.cpp#L105
// Why is that? well anyway this task set's it and exits
TaskHandle_t priorityLoopHandle;
void priorityLoopSet(void *parameter)
{
  while (true)
  {
    TaskHandle_t loopTaskHandle = xTaskGetHandle("loopTask");
    if (loopTaskHandle != NULL)
    {
      vTaskPrioritySet(loopTaskHandle, MAIN_LOOP_PRIORITY);
      // debugLog("Set loop task priority, exiting...");
      vTaskDelete(NULL);
    }
    delayTask(350);
  }
}

void setup()
{
#if DEBUG
#if PUT_LOGS_TO_SERIAL // This is here first because of watchy 3
  Serial.begin(SERIAL_BAUDRATE);
#if ATCHY_VER == WATCHY_3 || ATCHY_VER == YATCHY
  Serial.setDebugOutput(true);
#endif
#endif
#if WAIT_FOR_MONITOR
  delay(7500);
#endif
#if WAIT_FOR_INPUT
  int theDelay = 500;
#if WAIT_FOR_INPUT_MOTOR
  pinMode(VIB_MOTOR_PIN, OUTPUT);
#endif
  Serial.flush();
  while (true)
  {
    delayTask(theDelay / 2);
    if (Serial.available() > 0 && Serial.readString().indexOf("123") >= 0)
    {
      Serial.println("Received input! launching in 3..");
      delayTask(theDelay);
      Serial.println("2...");
      delayTask(theDelay);
      Serial.println("1...");
      delayTask(theDelay);
      Serial.println("Go!");
      break;
    }
#if WAIT_FOR_INPUT_MOTOR
    digitalWrite(VIB_MOTOR_PIN, 1);
    delayTask(theDelay / 10);
    digitalWrite(VIB_MOTOR_PIN, 0);
#endif
#if ATCHY_VER == YATCHY
    // usb_serial_jtag_driver_uninstall doesn't help
    // if it can't connect after deep sleep consider using a better usb connection
    if (usb_serial_jtag_is_connected() == false)
    {
      delayTask(1000);
    }
#endif
    Serial.println("Waiting for input...");
  }
  digitalWrite(VIB_MOTOR_PIN, 0);
#endif
  initLogs();
#endif

  initHardware();
  // debugLog("Starting millis: " + String(millisBetter()));

#if DEBUG
#if DUMP_INIT_DEBUG
  initHardwareDebug();
  loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
  loopGeneralDebug();
#endif
#if VOLTAGE_PRINT_ON
  xTaskCreate(
      dumpBatteryScreen,
      "dumpBatteryTask",
      TASK_STACK_DUMP_BATTERY,
      NULL,
      0,
      nullptr);
#endif

#if SCOM_TASK_ENABLED
  initScomTask();
#endif
#endif

  initManager();

  if (bootStatus.reason != rtc)
  {
    // I trust myself enough now to not need watchdog task running all the time
    initWatchdogTask();
    watchdogPing();

    turnOnButtons();

    xTaskCreate(
        priorityLoopSet,
        "priorityLoop",
        1000,
        NULL,
        20,
        &priorityLoopHandle);
  }

}

void loop()
{
#if ATCHY_VER == YATCHY
  if (bat.isCharging == true || bat.isFullyCharged == true)
  {
    isChargingCheck();
  }
#endif
#if TEMP_CHECKS_ENABLED
  tempChecker();
#endif
  if (bootStatus.reason != rtc)
  {
    watchdogPing();
    alarmManageRTC();
  }
  loopManager();

#if !DEBUG || !NO_SYNC
  regularSync();
#endif

#if DEBUG && EINK_COUNTER
  showEinkCounter();
#endif

#if DEBUG && SPEED_THROUGH_TIME
  debugLog("Speeding time");
  timeRTCUTC0.Minute = timeRTCUTC0.Minute + 1;
  if (String(timeRTCUTC0.Minute).indexOf("5") != -1)
  {
    timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
    timeRTCUTC0.Day = timeRTCUTC0.Day + 1;
    // timeRTCUTC0.Month = timeRTCUTC0.Month + 1; // We rely on previous day to clean up so this makes things break
  }
  if (timeRTCUTC0.Minute == 60)
  {
    timeRTCUTC0.Minute = 0;
    timeRTCUTC0.Hour = timeRTCUTC0.Hour + 1;
  }
  if (timeRTCUTC0.Hour == 24)
  {
    timeRTCUTC0.Hour = 0;
  }
  if (timeRTCUTC0.Day == 32)
  {
    timeRTCUTC0.Day = 1;
  }
  if (timeRTCUTC0.Month == 12)
  {
    timeRTCUTC0.Month = 0;
  }
  saveRTC(timeRTCUTC0);
  return;
#endif

#if DEBUG && (DUMP_LOOP_DEBUG || DUMP_LOOP_SOFTWARE_DEBUG)
  if (millisBetter() - loopDumpDelayMs > DUMP_LOOP_DELAY)
  {
    loopDumpDelayMs = millisBetter();
#if DUMP_LOOP_DEBUG
    loopHardwareDebug();
#endif
#if DUMP_LOOP_SOFTWARE_DEBUG
    loopGeneralDebug();
#endif
  }
#endif

#if !DISABLE_SLEEP
  manageSleep();
#endif
}
