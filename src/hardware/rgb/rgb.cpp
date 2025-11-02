#include "rgb.h"
#include "rtcMem.h"

uint rgbTimer = 0;
TaskHandle_t rgbTask = NULL;
bool rgbTaskRunning = false;
std::mutex rgbTaskMutex;
int ledOnTime = 0;

std::mutex currentColorMutex;
IWColors currentColor = IWColors::IwNone;

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

#if ATCHY_VER == YATCHY
void setRgbPullUp(bool state)
{
    rM.gpioExpander.setPinPullUp(RGB_DIODE_RED_PIN, state);
    rM.gpioExpander.setPinPullUp(RGB_DIODE_GREEN_PIN, state);
    rM.gpioExpander.setPinPullUp(RGB_DIODE_BLUE_PIN, state);
}
#endif

void setRgb(IWColors color, bool clearPrevious, uint timeMs)
{
#if RGB_DIODE
#if ATCHY_VER == YATCHY
    if (rM.gpioExpander.simplerInit() == false)
    {
        return;
    }
    if (clearPrevious == true && color != IwNone)
    {
        setRgb(IwNone, false);
    }
    if (color != IwNone && timeMs != 0)
    {
        rgbTaskMutex.lock();
        if (rgbTaskRunning == false)
        {
            rgbTaskRunning = true;
            rgbTimer = timeMs + 50; // 50 because we are turning the diode soon
            debugLog("Launching RGB task");
            xTaskCreate(
                rgbTaskRun,
                "rgbTask",
                TASK_STACK_RGB, // Too much but handling fs logging takes a bit more
                NULL,
                RGB_PRIORITY,
                &rgbTask);
        } else {
            debugLog("Rgb fallback");
            setRgb(IwNone, false);
        }
        rgbTaskMutex.unlock();
    }

    bool lockedColor = false;
    if (currentColorMutex.try_lock() == true)
    {
        lockedColor = true;
        currentColor = color;
    }

    if (color != IwNone)
    {
        setRgbPullUp(false);
        ledOnTime = 0;
    }

    switch (color)
    {
    case IwNone:
    {
        rM.gpioExpander.setPinState(RGB_DIODE_RED_PIN, true);
        rM.gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, true);
        rM.gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, true);
        // Weird but it fixes high power consumption
        setRgbPullUp(true);
        break;
    }
    case IwRed:
    {
        rM.gpioExpander.setPinState(RGB_DIODE_RED_PIN, false);
        break;
    }
    case IwGreen:
    {
        rM.gpioExpander.setPinState(RGB_DIODE_GREEN_PIN, false);
        break;
    }
    case IwBlue:
    {
        rM.gpioExpander.setPinState(RGB_DIODE_BLUE_PIN, false);
        break;
    }
    case IwYellow:
    {
        setRgb(IwRed, true);
        setRgb(IwGreen, false);
        break;
    }
    case IwPink:
    {
        setRgb(IwRed, true);
        setRgb(IwBlue, false);
        break;
    }
    case IwCyan:
    {
        setRgb(IwGreen, true);
        setRgb(IwBlue, false);
        break;
    }
    case IwWhite:
    {
        setRgb(IwRed, true);
        setRgb(IwBlue, false);
        setRgb(IwGreen, false);
        break;
    }
    }

    if (lockedColor == true)
    {
        currentColorMutex.unlock();
    }
#endif
#endif
}
