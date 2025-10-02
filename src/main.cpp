#include "defines.h"
#include "rtcMem.h"

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

  debugLog("Starting Tamp compression/decompression test...");

  const char* test_data_str = "This is a test string for Tamp compression. It should be compressed and then decompressed correctly.";
  uint8_t* original_data = (uint8_t*)test_data_str;
  int original_size = strlen(test_data_str) + 1; // +1 for null terminator

  String test_blob_name = "/test_blob.tamp";
  String test_dir = "/conf/";

  // Set blob (compress and write)
  if (fsSetBlob(test_blob_name, original_data, original_size, test_dir)) {
    debugLog("fsSetBlob successful for " + test_blob_name);

    // Get blob (read and decompress)
    bufSize retrieved_blob = fsGetBlob(test_blob_name, test_dir);

    if (retrieved_blob.buf != nullptr && retrieved_blob.size > 0) {
      debugLog("fsGetBlob successful for " + test_blob_name + ", decompressed size: " + String(retrieved_blob.size));

      // Compare original and decompressed data
      if (retrieved_blob.size == original_size && memcmp(original_data, retrieved_blob.buf, original_size) == 0) {
        debugLog("Tamp test PASSED: Original and decompressed data match!");
      } else {
        debugLog("Tamp test FAILED: Original and decompressed data DO NOT match!");
        debugLog("Original: " + String((char*)original_data));
        debugLog("Decompressed: " + String((char*)retrieved_blob.buf));
      }
      free(retrieved_blob.buf); // Free the allocated buffer
    } else {
      debugLog("Tamp test FAILED: fsGetBlob returned empty buffer.");
    }
  } else {
    debugLog("Tamp test FAILED: fsSetBlob failed.");
  }
  debugLog("Tamp compression/decompression test finished.");

  debugLog("Starting Python-generated TAMP compression test...");

  const char* python_test_data_str = "This is a test string for Python-generated TAMP compression. It should be compressed by the Python script and then decompressed correctly by the C++ code.";
  uint8_t* python_original_data = (uint8_t*)python_test_data_str;
  int python_original_size = strlen(python_test_data_str) + 1; // +1 for null terminator

  String python_test_blob_name = "/test_python_compression.txt";
  String python_test_dir = "/"; // Assuming it's in the root of littlefs

  // Get blob (read and decompress)
  bufSize python_retrieved_blob = fsGetBlob(python_test_blob_name, python_test_dir);

  if (python_retrieved_blob.buf != nullptr && python_retrieved_blob.size > 0) {
    debugLog("fsGetBlob successful for Python-generated " + python_test_blob_name + ", decompressed size: " + String(python_retrieved_blob.size));

    // Compare original and decompressed data
    if (python_retrieved_blob.size == python_original_size && memcmp(python_original_data, python_retrieved_blob.buf, python_original_size) == 0) {
      debugLog("Python-generated TAMP test PASSED: Original and decompressed data match!");
    } else {
      debugLog("Python-generated TAMP test FAILED: Original and decompressed data DO NOT match!");
      debugLog("Original: " + String((char*)python_original_data));
      debugLog("Decompressed: " + String((char*)python_retrieved_blob.buf));
    }
    free(python_retrieved_blob.buf); // Free the allocated buffer
  } else {
    debugLog("Python-generated TAMP test FAILED: fsGetBlob returned empty buffer or failed.");
  }
  debugLog("Python-generated TAMP compression test finished.");

  delayTask(9999999);
#endif
  
  initHardware();
  // debugLog("Starting millis: " + String(millisBetter()));

#if DEBUG
  showInitLogs();
#endif

  initManager();

  if (isFullMode() == true)
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

#if INK_ALARMS
  checkAlarms();
#endif
}

void loop()
{
  if (isFullMode() == true)
  {
    watchdogPing();
    manageRTC();
#if INK_ALARMS
    checkAlarms();
#endif
    loopBattery(false);
#if !DEBUG || !NO_SYNC
    regularSync();
#endif
  }
#if TEMP_CHECKS_ENABLED
  tempChecker();
#endif

  loopManager();

#if DEBUG
  endLoopDebug();
#endif

#if !DISABLE_SLEEP || !DEBUG
  manageSleep();
#endif
}
