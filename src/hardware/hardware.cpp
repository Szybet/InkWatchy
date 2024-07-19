#include "hardware.h"

RTC_DATA_ATTR float HWVer;
int64_t sleepDelayMs;
#define FIRST_BOOT_FILE "first_boot"

#if DEBUG
uint64_t loopDumpDelayMs;
#endif

// Also at boot, but on wake up too
void initHardware(bool isFromWakeUp, esp_sleep_wakeup_cause_t wakeUpReason)
{
    if (isFromWakeUp == false)
    {
        debugLog("Watchy is starting!");

        if (fsSetup() == true)
        {
            int firstBoot = fsGetString(FIRST_BOOT_FILE, "0").toInt();
            if (firstBoot < 1)
            {
                if (fsSetString(FIRST_BOOT_FILE, String(firstBoot + 1)) == true)
                {
                    if (fsGetString(FIRST_BOOT_FILE, "0").toInt() == firstBoot + 1)
                    {
                        debugLog("This is the first boot. Clearing core dump and nvs partition");
                        debugLog("esp_core_dump_image_erase status: " + String(esp_err_to_name(esp_core_dump_image_erase())));
                        debugLog("nvs_flash_erase status: " + String(esp_err_to_name(nvs_flash_erase())));
                        // This may be needed to avoid weird watchdog resets?
                        delay(1500);
                        ESP.restart();
                    }
                    else
                    {
                        debugLog("Failed to write a file to littlefs but no errors reported? fuck...");
                    }
                }
                else
                {
                    debugLog("Failed to set first boot file string, little fs is borked?");
                }
            }
        }
    }
    else
    {
        debugLog("Watchy is waking up!");
    }

#if DEBUG == 1
    setCpuSpeed(DEBUG_CPU_SPEED);
#else
    setCpuSpeed(CPU_SPEED);
#endif

    initRTC(isFromWakeUp, wakeUpReason);
    initButtons(isFromWakeUp);
    if (isFromWakeUp == false)
    {
        loadAllStorage();
        initBattery();
    }
    /*
    // Not available :(
    esp_pm_config_esp32_t pm_config = {
        .max_freq_mhz = 240,        // Set the maximum CPU frequency to 80 MHz
        .min_freq_mhz = 240,        // Set the minimum CPU frequency to 40 MHz
        .light_sleep_enable = false // Enable light sleep
    };
    esp_err_t status = esp_pm_configure(&pm_config);
    debugLog("Configuring pm status: " + String(esp_err_to_name(status)));
    */

    pinMode(VIB_MOTOR_PIN, OUTPUT);
    digitalWrite(VIB_MOTOR_PIN, false); // To reset the motor button if esp crashed when it was vibrating

    initDisplay(isFromWakeUp);
    resetSleepDelay();
}

void resetSleepDelay(int addMs)
{
    sleepDelayMs = millisBetter() + addMs;
}

void setSleepDelay(int addMs)
{
    sleepDelayMs = millisBetter() - (SLEEP_EVERY_MS - addMs);
    // debugLog("setSleepDelay sleepDelayMs: " + String(sleepDelayMs));
}

#if DEBUG
void initHardwareDebug()
{
    initRTCDebug();
    debugLog("Hardware version: " + String(HWVer));
    debugLog("Up button pin number: " + String(UP_PIN));
    initDisplayDebug();
    initGeneralDebug();
}

void loopHardwareDebug()
{
    loopRTCDebug();
    dumpRTCTime();
    dumpButtons();
    dumpBattery();
    loopGeneralDebug();
}
#endif

TaskHandle_t motorTask = NULL;
bool motorTaskRunning = false;
int vibrateTime;
void vibrateMotorTaskFun(void *parameter)
{
    motorTaskRunning = true;
    debugLog("Motor on");
    int vibrateTimeout = vibrateTime / VIBRATION_DIVINE;
    debugLog("vibrateTime: " + String(vibrateTime) + " vibrateTimeout: " + String(vibrateTimeout) + " VIBRATION_DIVINE: " + String(VIBRATION_DIVINE));
    for (int i = 0; i < vibrateTime / VIBRATION_DIVINE; i++)
    {
        digitalWrite(VIB_MOTOR_PIN, true);
        delayTask(vibrateTimeout);
        digitalWrite(VIB_MOTOR_PIN, false);
        delayTask(vibrateTimeout);
    }
    debugLog("Motor off");
    motorTaskRunning = false;
    vTaskDelete(NULL);
}

void vibrateMotor(int vTime, bool add)
{
    if (disableAllVibration == true)
    {
        debugLog("Vibrations are disabled");
        return;
    }

    if (motorTaskRunning == false)
    {
        debugLog("Creating motor task");
        vibrateTime = vTime;
        xTaskCreate(
            vibrateMotorTaskFun,
            "motorTask",
            TASK_STACK_VIBRATION,
            NULL,
            MOTOR_PRIORITY,
            &motorTask);
    }
    if (add == true && motorTaskRunning == true)
    {
        debugLog("Adding time to motor");
        vibrateTime = vibrateTime + vTime;
    }
    debugLog("Mottor task done");
}

String resetReasonToString(esp_reset_reason_t reason)
{
    switch (reason)
    {
    case ESP_RST_UNKNOWN:
        return "ESP_RST_UNKNOWN"; // Reset reason cannot be determined
    case ESP_RST_POWERON:
        return "ESP_RST_POWERON"; // Reset due to power-on event
    case ESP_RST_EXT:
        return "ESP_RST_EXT"; // Reset by external pin (not applicable for ESP32)
    case ESP_RST_SW:
        return "ESP_RST_SW"; // Software reset via esp_restart
    case ESP_RST_PANIC:
        return "ESP_RST_PANIC"; // Software reset due to exception/panic
    case ESP_RST_INT_WDT:
        return "ESP_RST_INT_WDT"; // Reset (software or hardware) due to interrupt watchdog
    case ESP_RST_TASK_WDT:
        return "ESP_RST_TASK_WDT"; // Reset due to task watchdog
    case ESP_RST_WDT:
        return "ESP_RST_WDT"; // Reset due to other watchdogs
    case ESP_RST_DEEPSLEEP:
        return "ESP_RST_DEEPSLEEP"; // Reset after exiting deep sleep mode
    case ESP_RST_BROWNOUT:
        return "ESP_RST_BROWNOUT"; // Brownout reset (software or hardware)
    case ESP_RST_SDIO:
        return "ESP_RST_SDIO"; // Reset over SDIO
    default:
        return "UNKNOWN"; // Unknown reset reason
    }
}

cpuSpeed savedCpuSpeed = minimalSpeed;
void setCpuSpeed(cpuSpeed speed)
{
    // Only these values are available
    switch (speed)
    {
    case minimalSpeed:
    {
        setCpuFrequencyMhz(80);
        break;
    }
    case normalSpeed:
    {
        setCpuFrequencyMhz(160);
        break;
    }
    case maxSpeed:
    {
        setCpuFrequencyMhz(240);
        break;
    }
    }
}

cpuSpeed getCpuSpeed()
{
    uint32_t cpuMhz = getCpuFrequencyMhz();
    debugLog("cpu Mhz bare: " + String(cpuMhz));
    switch (cpuMhz)
    {
    case 80:
    {
        savedCpuSpeed = minimalSpeed;
        return minimalSpeed;
    }
    case 160:
    {
        savedCpuSpeed = normalSpeed;
        return normalSpeed;
    }
    case 240:
    {
        savedCpuSpeed = maxSpeed;
        return maxSpeed;
    }
    }
    debugLog("Something went wrong with cpu speed");
    savedCpuSpeed = minimalSpeed;
    return minimalSpeed;
}

void restoreCpuSpeed()
{
    setCpuSpeed(savedCpuSpeed);
}

void softStartDelay()
{
#if SOFT_START_THINGS
    delayTask(SOFT_START_DELAY_MS);
#endif
}

#if DEBUG
String wakeupSourceToString(esp_sleep_source_t source)
{
    switch (source)
    {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
        return "ESP_SLEEP_WAKEUP_UNDEFINED";
    case ESP_SLEEP_WAKEUP_ALL:
        return "ESP_SLEEP_WAKEUP_ALL";
    case ESP_SLEEP_WAKEUP_EXT0:
        return "ESP_SLEEP_WAKEUP_EXT0";
    case ESP_SLEEP_WAKEUP_EXT1:
        return "ESP_SLEEP_WAKEUP_EXT1";
    case ESP_SLEEP_WAKEUP_TIMER:
        return "ESP_SLEEP_WAKEUP_TIMER";
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        return "ESP_SLEEP_WAKEUP_TOUCHPAD";
    case ESP_SLEEP_WAKEUP_ULP:
        return "ESP_SLEEP_WAKEUP_ULP";
    case ESP_SLEEP_WAKEUP_GPIO:
        return "ESP_SLEEP_WAKEUP_GPIO";
    case ESP_SLEEP_WAKEUP_UART:
        return "ESP_SLEEP_WAKEUP_UART";
    case ESP_SLEEP_WAKEUP_WIFI:
        return "ESP_SLEEP_WAKEUP_WIFI";
    case ESP_SLEEP_WAKEUP_COCPU:
        return "ESP_SLEEP_WAKEUP_COCPU";
    case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG:
        return "ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG";
    case ESP_SLEEP_WAKEUP_BT:
        return "ESP_SLEEP_WAKEUP_BT";
    default:
        return "UNKNOWN";
    }
}
#endif

// int because we want, when - it to not overlap
// And I don't trust arduino functions
int64_t millisBetter()
{
    return esp_timer_get_time() / 1000ULL;
}

bool isRtcWakeUpReason(esp_sleep_source_t reason)
{
    if (reason == ESP_SLEEP_WAKEUP_EXT0 || reason == ESP_SLEEP_WAKEUP_TIMER)
    {
        return true;
    }
    return false;
}