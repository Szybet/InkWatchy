#include "buttons.h"
#include "rtcMem.h"

buttonState interruptedButton = None;

// No logs here because interrupts don't like them
void resumeButtonTask()
{
    // debugLog("Resuming button task");
    eTaskState taskState = eTaskGetState(buttonTask);
    if (taskState == eSuspended)
    {
        vTaskResume(buttonTask);
    }
#if DEBUG
    else
    {
        if (taskState != eRunning)
        {
            // debugLog("Something is wrong with button task: " + String(taskState));
        }
    }
#endif
}

#if ATCHY_VER != YATCHY
void IRAM_ATTR backIntBut()
{
    interruptedButton = Back;
    resumeButtonTask();
}

void IRAM_ATTR menuIntBut()
{
    interruptedButton = Menu;
    resumeButtonTask();
}

void IRAM_ATTR upIntBut()
{
    interruptedButton = Up;
    resumeButtonTask();
}

void IRAM_ATTR downIntBut()
{
    interruptedButton = Down;
    resumeButtonTask();
}
#endif

void turnOnInterrupts()
{
#if ATCHY_VER != YATCHY
    attachInterrupt(digitalPinToInterrupt(BACK_PIN), backIntBut, BUTTON_INTER_COND);
    attachInterrupt(digitalPinToInterrupt(MENU_PIN), menuIntBut, BUTTON_INTER_COND);
    attachInterrupt(digitalPinToInterrupt(UP_PIN), upIntBut, BUTTON_INTER_COND);
    attachInterrupt(digitalPinToInterrupt(DOWN_PIN), downIntBut, BUTTON_INTER_COND);
#else
    rM.gpioExpander.interruptsManage(true);
#endif
    debugLog("Attached interrupts!");
}

void turnOffInterrupts()
{
#if ATCHY_VER != YATCHY
    detachInterrupt(digitalPinToInterrupt(BACK_PIN));
    detachInterrupt(digitalPinToInterrupt(MENU_PIN));
    detachInterrupt(digitalPinToInterrupt(UP_PIN));
    detachInterrupt(digitalPinToInterrupt(DOWN_PIN));
#else
    rM.gpioExpander.interruptsManage(false);
#endif
    debugLog("Deattached interrupts!");
}
