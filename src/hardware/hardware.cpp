#include "hardware.h"
#include "rtcMem.h"

int64_t sleepDelayMs;
#define FIRST_BOOT_FILE "first_boot"

#if DEBUG
uint64_t loopDumpDelayMs;
#endif

wakeUpInfo bootStatus = {};

// Also at boot, but on wake up too
void initHardware()
{
    // To turn it off if something bad happens
    initMotor();

    bootStatus.bareEspCause = esp_sleep_get_wakeup_cause();
    bootStatus.resetReason = esp_reset_reason();
    debugLog("Sleep wakeup reason: " + wakeupSourceToString(bootStatus.bareEspCause));
    if (bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_EXT0 || bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_TIMER)
    {
        debugLog("Waked up because of RTC");
        bootStatus.fromWakeup = true;
        bootStatus.reason = rtc;
    }
    else if (bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_EXT1)
    {
        debugLog("Waked up because of ext1");
        debugLog("esp_sleep_get_ext1_wakeup_status: " + String(esp_sleep_get_ext1_wakeup_status()));
        bootStatus.fromWakeup = true;
        bootStatus.reason = button;
        manageButtonWakeUp();
    }
    #if ATCHY_VER == YATCHY
    else if(bootStatus.bareEspCause == ESP_SLEEP_WAKEUP_ULP) {
        bootStatus.fromWakeup = true;
        bootStatus.reason = ulp;
    }
    #endif

    if (bootStatus.fromWakeup == false)
    {
        debugLog("Watchy is starting!");
        debugLog("RTC memory used: " + String(sizeof(rtcMem)) + " bytes");
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
    if (isFullMode() == true)
    {
        setCpuSpeed(BUTTON_CPU_SPEED);
    }
    else
    {
        setCpuSpeed(CPU_SPEED);
    }
#endif

#if ATCHY_VER == YATCHY
#if LP_CORE
    // Always, to be sure
    stopLpCore();
#else
    deInitRtcGpio();
#endif
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
    }

    initBattery();

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

    initDisplay();

#if LP_CORE
    initManageLpCore();
#endif
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
    case ESP_RST_USB:
        return "ESP_RST_USB"; // Reset by USB peripheral
    case ESP_RST_JTAG:
        return "ESP_RST_JTAG"; // Reset by JTAG
    case ESP_RST_EFUSE:
        return "ESP_RST_EFUSE"; // Reset due to efuse error
    case ESP_RST_PWR_GLITCH:
        return "ESP_RST_PWR_GLITCH"; // Reset due to power glitch detected
    case ESP_RST_CPU_LOCKUP:
        return "ESP_RST_CPU_LOCKUP"; // Reset due to CPU lock up
    default:
        return "REALLY_UNKNOWN_:P"; // Unknown reset reason
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
        setCpuFrequencyMhz(CPU_MINIMAL_SPEED);
        break;
    }
    case normalSpeed:
    {
        setCpuFrequencyMhz(CPU_NORMAL_SPEED);
        break;
    }
    case maxSpeed:
    {
        setCpuFrequencyMhz(CPU_FAST_SPEED);
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
#if DEBUG == true && LP_CORE == true
    debugLog("This is a warning, first boot detected but nvs and coredump not cleared because of debug and lp core");
#else
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
#endif
}

bool isFullMode() {
    // For things that aren't needed if it's just wakeup - update - sleep
    if(bootStatus.reason == button) {
        // debugLog("Full mode because of button");
        return true;
    }
    if(millisBetter() > FULL_MODE_AFTER_S * 1000) {
        // debugLog("Full mode because of time passed");
        return true;
    }
    if(rM.bat.isCharging == true || rM.bat.isFullyCharged == true) {
        // debugLog("Full mode because of battery charging or is fully charged");
        return true;
    }

    // False now
    if(bootStatus.reason == wakeUpReason::rtc || bootStatus.reason == wakeUpReason::ulp) {
        // debugLog("Power mode because of rtc or ulp wakeup");
        return false;
    }

    // debugLog("Power mode because everything else failed, this should NOT happen");
    return false;
}