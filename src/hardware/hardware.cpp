#include "hardware.h"

int64_t sleepDelayMs;
#define FIRST_BOOT_FILE "first_boot"

#if DEBUG
uint64_t loopDumpDelayMs;
#endif

wakeUpInfo bootStatus = {};

// Also at boot, but on wake up too
void initHardware()
{

    bootStatus.bareEspCause = esp_sleep_get_wakeup_cause();
    bootStatus.resetReason = esp_reset_reason();
    if (bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_EXT0 || bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_TIMER)
    {
        debugLog("Waked up because of RTC");
        bootStatus.fromWakeup = true;
        bootStatus.reason = rtc;
    }
    else if (bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_EXT1)
    {
        debugLog("Waked up because of buttons");
        debugLog("esp_sleep_get_ext1_wakeup_status: " + String(esp_sleep_get_ext1_wakeup_status()));
        bootStatus.fromWakeup = true;
        bootStatus.reason = button;
        manageButtonWakeUp();
    }

    if (bootStatus.fromWakeup == false)
    {
        debugLog("Watchy is starting!");
        firstWakeUpManage();
    }
    else
    {
        debugLog("Watchy is waking up!");
        debugLog("Sleep wakeup reason: " + wakeupSourceToString(bootStatus.bareEspCause));
    }

#if DEBUG == 1
    setCpuSpeed(DEBUG_CPU_SPEED);
#else
    setCpuSpeed(CPU_SPEED);
#endif

    initRTC();
    initButtons();

    // Before initBattery, but executed always
#if ATCHY_VER == WATCHY_3
    pinMode(CHRG_STATUS_PIN, INPUT);
#endif

    if (bootStatus.fromWakeup == false)
    {
        loadAllStorage();
        initBattery();
    }
    else
    {
        // This is for RTC wakeup
        // We could put loop battery inside init battery and pass a bool, but for now this
        loopBattery();
    }

    /*
    // Implement in the future?
    esp_pm_config_esp32_t pm_config = {
        .max_freq_mhz = 240,        // Set the maximum CPU frequency to 80 MHz
        .min_freq_mhz = 240,        // Set the minimum CPU frequency to 40 MHz
        .light_sleep_enable = false // Enable light sleep
    };
    esp_err_t status = esp_pm_configure(&pm_config);
    debugLog("Configuring pm status: " + String(esp_err_to_name(status)));
    */

    setRgb(IwGreen);
    setRgb(IwRed);
    setRgb(IwBlue);

    initDisplay();
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
    initDisplayDebug();
    initGeneralDebug();
}

void loopHardwareDebug()
{
    loopRTCDebug();
    dumpRTCTime(timeRTCLocal);
    dumpButtons();
    dumpBattery();
    loopGeneralDebug();
}
#endif

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
    if (getCpuSpeed() == speed)
    {
        return;
    }
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

void firstWakeUpManage()
{
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