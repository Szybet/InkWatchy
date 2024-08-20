#include "buttons.h"

bool buttonsActivated = false;
buttonState buttonPressed = None;
TaskHandle_t buttonTask = NULL;
std::mutex buttMut;

buttonState useButtonBack()
{
    buttMut.lock();
    // debugLog("useButtonBack state: " + getButtonString(buttonPressed));
    if (buttonPressed == Back || buttonPressed == LongBack)
    {
        buttonState buttonPressedTmp = buttonPressed;
        buttonPressed = None;
        buttMut.unlock();
        return buttonPressedTmp;
    }
    buttMut.unlock();
    return None;
}

buttonState useButton()
{
    buttMut.lock();
    if (buttonPressed == Back || buttonPressed == LongBack)
    {
        buttMut.unlock();
        return None;
    }
    buttonState buttonPressedTmp = buttonPressed;
    buttonPressed = None;

    // debugLog("Used button by UI: " + getButtonString(buttonPressedTmp));
    if (buttonPressedTmp != None)
    {
        debugLog("Used button by UI: " + getButtonString(buttonPressedTmp));
    }
    buttMut.unlock();
    return buttonPressedTmp;
}

// Should be used only in watchface
buttonState useAllButtons()
{
    buttMut.lock();
    // if (buttonPressed == Back || buttonPressed == LongBack)
    // {
    //     buttMut.unlock();
    //     return None;
    // }
    buttonState buttonPressedTmp = buttonPressed;
    buttonPressed = None;

    // debugLog("Used button by UI: " + getButtonString(buttonPressedTmp));
    if (buttonPressedTmp != None)
    {
        debugLog("Used button by UI: " + getButtonString(buttonPressedTmp));
    }
    buttMut.unlock();
    return buttonPressedTmp;
}

// To unlock the button
void useButtonBlank()
{
    buttMut.lock();
    if (buttonPressed != Back && buttonPressed != LongBack)
    {
        buttonPressed = None;
    }
    buttMut.unlock();
}

void initButtons(bool isFromWakeUp)
{
#if ATCHY_VER == WATCHY_3
    rtc_gpio_set_direction((gpio_num_t)UP_PIN, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pullup_en((gpio_num_t)UP_PIN);
#endif
#if ATCHY_VER != YATCHY
    pinMode(MENU_PIN, INPUT);
    pinMode(BACK_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(DOWN_PIN, INPUT);
#endif
#if ATCHY_VER == WATCHY_3
    rtc_gpio_set_direction((gpio_num_t)UP_PIN, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pullup_en((gpio_num_t)UP_PIN);
#endif
}

void setButton(buttonState button)
{
    buttMut.lock();
    debugLog("setButton called: " + getButtonString(button));
    buttonPressed = button;
    buttMut.unlock();
    vibrateMotor();
    resetSleepDelay();
    debugLog("setButton done");
}

void longButtonCheck(int buttonPin, buttonState normalButton, buttonState longButton)
{
    int startime = millisBetter();
    int elapsedtime = 0;
    while (buttonRead(buttonPin) == BUT_CLICK_STATE && elapsedtime < BUTTON_LONG_PRESS_MS)
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
        elapsedtime = millisBetter() - startime;
    }
    debugLog("elapsed time: " + String(elapsedtime) + " BUTTON_LONG_PRESS_MS:" + String(BUTTON_LONG_PRESS_MS));
    if (elapsedtime > BUTTON_LONG_PRESS_MS)
    {
        setButton(longButton);
        vibrateMotor(VIBRATION_BUTTON_TIME * 1.7, true);
        while (buttonRead(buttonPin) == BUT_CLICK_STATE)
        {
            delayTask(SMALL_BUTTON_DELAY_MS);
        }
    }
    else
    {
        setButton(normalButton);
    }
    delayTask(BUTTON_TASK_DELAY / ADD_BUTTON_DELAY);
}

#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_3 || ATCHY_VER == YATCHY
void loopButtonsTask(void *parameter)
{
    buttonsActivated = true;
    // Wait for all buttons to drop down, helpfull for manageButtonWakeUp
    while (buttonRead(BACK_PIN) == BUT_CLICK_STATE || buttonRead(MENU_PIN) == BUT_CLICK_STATE || buttonRead(UP_PIN) == BUT_CLICK_STATE || buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
    }
    interruptedButton = None;
    while (true)
    {
        // debugLog("Button task looping...");
        buttonState interruptedButtonCopy = interruptedButton;
        // debugLog("interruptedButtonCopy: " + getButtonString(interruptedButtonCopy));
        // debugLog("buttonPressed: " + getButtonString(buttonPressed));

#if ATCHY_VER == YATCHY
        if (interruptedButtonCopy == Unknown)
        {
#if ATCHY_VER == YATCHY
            interruptedButtonCopy = gpioExpander.manageInterrupts();
            interruptedButton = interruptedButtonCopy; // to make sure it can "rest" at the end
            debugLog("Received button from gpio expander: " + getButtonString(buttonPressed));
#endif
        }
#endif

        buttMut.lock();
        if (interruptedButtonCopy == Back && buttonPressed != LongBack)
        {
            buttMut.unlock();
            longButtonCheck(BACK_PIN, Back, LongBack);
            buttMut.lock();
        }
        else if (interruptedButtonCopy == Menu && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(MENU_PIN, Menu, LongMenu);
            buttMut.lock();
        }
        else if (interruptedButtonCopy == Up && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu && buttonPressed != LongUp)
        {
            buttMut.unlock();
            longButtonCheck(UP_PIN, Up, LongUp);
            buttMut.lock();
        }
        else if (interruptedButtonCopy == Down && buttonPressed != Up && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongUp && buttonPressed != LongBack && buttonPressed != LongDown && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(DOWN_PIN, Down, LongDown);
            buttMut.lock();
        }
        buttMut.unlock();
        if (interruptedButtonCopy == interruptedButton)
        {
            interruptedButton = None;
            #if ATCHY_VER == YATCHY
            gpioExpander.manageInterruptsExit();
            #endif
            debugLog("Button task going to sleep!"); // That's normal and very efficient
            vTaskSuspend(NULL);
        }
#if DEBUG
        else
        {
            debugLog("Another button clicked...");
        }
#endif
    }
}
// For some reason on those interrupt doesn't work
#elif ATCHY_VER == WATCHY_1 || ATCHY_VER == WATCHY_1_5
void loopButtonsTask(void *parameter)
{
    // Wait for all buttons to drop down, helpfull for manageButtonWakeUp
    while (buttonRead(BACK_PIN) == HIGH || buttonRead(MENU_PIN) == HIGH || buttonRead(UP_PIN) == HIGH || buttonRead(DOWN_PIN) == HIGH)
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
    }
    while (true)
    {
        delayTask(BUTTON_TASK_DELAY);
        buttMut.lock();
        if (buttonRead(BACK_PIN) == HIGH && buttonPressed != LongBack)
        {
            buttMut.unlock();
            longButtonCheck(BACK_PIN, Back, LongBack);
            buttMut.lock();
        }
        else if (buttonRead(MENU_PIN) == HIGH && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(MENU_PIN, Menu, LongMenu);
            buttMut.lock();
        }
        else if (buttonRead(UP_PIN) == HIGH && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu && buttonPressed != LongUp)
        {
            buttMut.unlock();
            longButtonCheck(UP_PIN, Up, LongUp);
            buttMut.lock();
        }
        else if (buttonRead(DOWN_PIN) == HIGH && buttonPressed != Up && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongUp && buttonPressed != LongBack && buttonPressed != LongDown && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(DOWN_PIN, Down, LongDown);
            buttMut.lock();
        }
        buttMut.unlock();
    }
}
#endif

void initButtonTask()
{
    xTaskCreate(
        loopButtonsTask,
        "buttonTask",
        TASK_STACK_BUTTON, // Too much but handling fs logging takes a bit more
        NULL,
        BUTTONS_PRIORITY,
        &buttonTask);
}
/*
Idk why this fuction doesn't work
==================== CURRENT THREAD STACK =====================
#0  0x400838dd in panic_abort (details=0x3ffb4090 "assert failed: eTaskGetState tasks.c:1696 (pxTCB)") at /root/.platformio/packages/framework-espidf/components/esp_system/panic.c:452
#1  0x4008bd88 in esp_system_abort (details=0x3ffb4090 "assert failed: eTaskGetState tasks.c:1696 (pxTCB)") at /root/.platformio/packages/framework-espidf/components/esp_system/port/esp_system_chip.c:84
#2  0x40092ff4 in __assert_func (file=<optimized out>, line=<optimized out>, func=<optimized out>, expr=<optimized out>) at /root/.platformio/packages/framework-espidf/components/newlib/assert.c:81
#3  0x4008d0fc in eTaskGetState (xTask=0x0) at /root/.platformio/packages/framework-espidf/components/freertos/FreeRTOS-Kernel/tasks.c:1696
#4  0x400d40b4 in deInitButtonTask () at src/hardware/buttons.cpp:182
#5  0x400d5426 in goSleep () at src/hardware/sleep.cpp:62
#6  0x400d551a in manageSleep () at src/hardware/sleep.cpp:148
#7  0x400d55bd in loop () at src/main.cpp:164
#8  0x400fa684 in loopTask (pvParameters=<optimized out>) at /root/.platformio/packages/framework-arduinoespressif32/cores/esp32/main.cpp:74
#9  0x4008ed1d in vPortTaskWrapper (pxCode=0x400fa650 <loopTask(void*)>, pvParameters=0x0) at /root/.platformio/packages/framework-espidf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:162

For now i will not care

MAYBE it's because this task is never initialized when doing a RTC wakeup...
*/
void deInitButtonTask()
{
    eTaskState taskState = eTaskGetState(buttonTask);
    if (taskState == eRunning || taskState == eSuspended)
    {
        debugLog("Shutting down button task");
        vTaskDelete(buttonTask);
    }
    else
    {
        debugLog("Not shutting down button task, it's state is: " + String(taskState));
    }
}

void wakeUpLong(int pin, buttonState normal, buttonState hold)
{
    long timeTime = millisBetter();

    while (buttonRead(pin) == BUT_CLICK_STATE && timeTime + BUTTON_LONG_PRESS_MS > millisBetter())
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
    }
    buttMut.lock();
    if (timeTime + BUTTON_LONG_PRESS_MS < millisBetter())
    {
        buttonPressed = hold;
    }
    else if (buttonRead(pin) == BUT_STATE)
    {
        buttonPressed = normal;
    }
    buttMut.unlock();
}

void manageButtonWakeUp()
{
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    vibrateMotor();
#if ATCHY_VER != YATCHY
    initButtons(true);
    uint64_t wakeupBit;
    wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if (wakeupBit & pinToMask(BACK_PIN))
    {
        wakeUpLong(BACK_PIN, Back, LongBack);
        return;
    }
    if (wakeupBit & pinToMask(MENU_PIN))
    {
        wakeUpLong(MENU_PIN, Menu, LongMenu);
        return;
    }
    if (wakeupBit & pinToMask(DOWN_PIN))
    {
        wakeUpLong(DOWN_PIN, Down, LongDown);
        return;
    }
    if (wakeupBit & pinToMask(UP_PIN))
    {
        wakeUpLong(UP_PIN, Up, LongUp);
        return;
    }
#else
    buttonState btn = gpioExpander.manageInterrupts();
#ifdef YATCHY_BACK_BTN
    if (btn == Back)
    {
        wakeUpLong(BACK_PIN, Back, LongBack);
        return;
    }
#endif
    if (btn == Menu)
    {
        wakeUpLong(MENU_PIN, Menu, LongMenu);
        return;
    }
    if (btn == Down)
    {
        wakeUpLong(DOWN_PIN, Down, LongDown);
        return;
    }
    if (btn == Up)
    {
        wakeUpLong(UP_PIN, Up, LongUp);
        return;
    }
#endif
}

#if DEBUG
void dumpButtons()
{
    if (buttonRead(MENU_PIN) == BUT_CLICK_STATE)
    {
        debugLog("Menu button pressed");
    }
    else if (buttonRead(BACK_PIN) == BUT_CLICK_STATE)
    {
        debugLog("Back button pressed");
    }
    else if (buttonRead(UP_PIN) == BUT_CLICK_STATE)
    {
        debugLog("Up button pressed");
    }
    else if (buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        debugLog("Down button pressed");
    }
}

String getButtonString(buttonState state)
{
    switch (state)
    {
    case Unknown:
        return "Unknown";
    case None:
        return "None";
    case Back:
        return "Back";
    case Menu:
        return "Menu";
    case Up:
        return "Up";
    case Down:
        return "Down";
    case LongBack:
        return "LongBack";
    case LongMenu:
        return "LongMenu";
    case LongUp:
        return "LongUp";
    case LongDown:
        return "LongDown";
    default:
        return "Error";
    }
}
#endif

void turnOnButtons()
{
    if (buttonsActivated == false)
    {
        initButtonTask();
#if ATCHY_VER == WATCHY_2 || ATCHY_VER == WATCHY_3 || ATCHY_VER == YATCHY
        turnOnInterrupts();
#endif
    }
}

bool buttonRead(uint8_t pin)
{
#if ATCHY_VER != YATCHY
    return digitalRead(pin);
#else
    return gpioExpander.digitalRead(pin);
#endif
}