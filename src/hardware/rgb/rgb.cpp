#include "rgb.h"

uint rgbTimer = 0;
TaskHandle_t rgbTask = NULL;
bool rgbTaskRunning = false;
std::mutex rgbTaskMutex;

void rgbTaskRun(void *parameter)
{
    rgbTaskMutex.lock();
    delayTask(rgbTimer);
    setRgb(IwNone, false, 0);
    rgbTimer = 0;
    rgbTaskRunning = false;
    rgbTaskMutex.unlock();
    vTaskDelete(NULL);
}

void setRgb(IWColors color, bool clearPrevious, uint timeMs)
{
#if RGB_DIODE
#if ATCHY_VER == YATCHY
    if (gpioExpander.simplerInit() == false)
    {
        return;
    }
    if (clearPrevious == true && color != IwNone)
    {
        setRgb(IwNone, false);
    }
    if (color != IwNone && timeMs != 0)
    {
        // This has a small possibility of ignoring the call, because it is already running
        if (rgbTaskMutex.try_lock() == true)
        {
            if (rgbTaskRunning == false)
            {
                rgbTaskRunning = true;
                rgbTimer = timeMs + 50; // 50 because we are turning the diode soon
                debugLog("Launching RGB task");
                rgbTaskMutex.unlock();
                xTaskCreate(
                    rgbTaskRun,
                    "rgbTask",
                    TASK_STACK_RGB, // Too much but handling fs logging takes a bit more
                    NULL,
                    RGB_PRIORITY,
                    &rgbTask);
            }
        }
    }
    switch (color)
    {
    case IwNone:
    {
        gpioExpander.setPinState(RGB_DIODE_RED_PIN, true);
        gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, true);
        gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, true);
        return;
    }
    case IwRed:
    {
        gpioExpander.setPinState(RGB_DIODE_RED_PIN, false);
        return;
    }
    case IwGreen:
    {
        gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, false);
        return;
    }
    case IwBlue:
    {
        gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, false);
        return;
    }
    case IwYellow:
    {
        setRgb(IwRed, true);
        setRgb(IwGreen, false);
        return;
    }
    case IwPink:
    {
        setRgb(IwRed, true);
        setRgb(IwBlue, false);
        return;
    }
    case IwCyan:
    {
        setRgb(IwGreen, true);
        setRgb(IwBlue, false);
        return;
    }
    case IwWhite:
    {
        setRgb(IwRed, true);
        setRgb(IwBlue, false);
        setRgb(IwGreen, false);
        return;
    }
    }
#endif
#endif
}