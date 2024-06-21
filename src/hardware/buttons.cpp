#include "buttons.h"

int RTC_DATA_ATTR UP_PIN = 32;
buttonState buttonPressed = None;
TaskHandle_t buttonTask = NULL;
std::mutex buttMut;

buttonState useButtonBack()
{
    buttMut.lock();
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
    if (isFromWakeUp == false)
    {
        if (SRTC.getType() == RTC_PCF8563)
        {
            if (HWVer == 1.5)
            {
                UP_PIN = 32;
            }
            else
            {
                UP_PIN = 35;
            }
        }
    }

    pinMode(MENU_PIN, INPUT);
    pinMode(BACK_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(DOWN_PIN, INPUT);
}

void setButton(buttonState button)
{
    buttMut.lock();
    debugLog("setButton called: " + getButtonString(button));
    vibrateMotor();
    buttonPressed = button;
    buttMut.unlock();
    resetSleepDelay();
    debugLog("setButton done");
}

void longButtonCheck(int buttonPin, buttonState normalButton, buttonState longButton)
{
    int startime = millis();
    int elapsedtime = 0;
    while (digitalRead(buttonPin) == HIGH && elapsedtime < BUTTON_LONG_PRESS_MS)
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
        elapsedtime = millis() - startime;
    }
    debugLog("elapsed time: " + String(elapsedtime) + " BUTTON_LONG_PRESS_MS:" + String(BUTTON_LONG_PRESS_MS));
    if (elapsedtime > BUTTON_LONG_PRESS_MS)
    {
        setButton(longButton);
        vibrateMotor(VIBRATION_BUTTON_TIME * 1.7, true);
        while (digitalRead(buttonPin) == HIGH)
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

void loopButtonsTask(void *parameter)
{
    // Wait for all buttons to drop down, helpfull for manageButtonWakeUp
    while (digitalRead(BACK_PIN) == HIGH || digitalRead(MENU_PIN) == HIGH || digitalRead(UP_PIN) == HIGH || digitalRead(DOWN_PIN) == HIGH)
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
    }
    while (true)
    {
        delayTask(BUTTON_TASK_DELAY);
        buttMut.lock();
        if (digitalRead(BACK_PIN) == HIGH && buttonPressed != LongBack)
        {
            buttMut.unlock();
            longButtonCheck(BACK_PIN, Back, LongBack);
            buttMut.lock();
        }
        else if (digitalRead(MENU_PIN) == HIGH && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(MENU_PIN, Menu, LongMenu);
            buttMut.lock();
        }
        else if (digitalRead(UP_PIN) == HIGH && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongBack && buttonPressed != LongMenu && buttonPressed != LongUp)
        {
            buttMut.unlock();
            longButtonCheck(UP_PIN, Up, LongUp);
            buttMut.lock();
        }
        else if (digitalRead(DOWN_PIN) == HIGH && buttonPressed != Up && buttonPressed != Menu && buttonPressed != Back && buttonPressed != LongUp && buttonPressed != LongBack && buttonPressed != LongDown && buttonPressed != LongMenu)
        {
            buttMut.unlock();
            longButtonCheck(DOWN_PIN, Down, LongDown);
            buttMut.lock();
        }
        buttMut.unlock();
    }
}

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

void deInitButtonTask()
{
    if (buttonTask != NULL)
    {
        debugLog("Shutting down button task");
        vTaskDelete(buttonTask);
        buttonTask = NULL;
    }
}

void wakeUpLong(int pin, buttonState normal, buttonState hold)
{
    long timeTime = millis();

    while (digitalRead(pin) == HIGH && timeTime + BUTTON_LONG_PRESS_MS > millis())
    {
        delayTask(SMALL_BUTTON_DELAY_MS);
    }
    buttMut.lock();
    if (timeTime + BUTTON_LONG_PRESS_MS < millis())
    {
        buttonPressed = hold;
    }
    else if (digitalRead(pin) == LOW)
    {
        buttonPressed = normal;
    }
    buttMut.unlock();
}

void manageButtonWakeUp()
{
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    vibrateMotor();
    initButtons(true);
    uint64_t wakeupBit;
    wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if (wakeupBit & BIT(BACK_PIN))
    {
        wakeUpLong(BACK_PIN, Back, LongBack);
        return;
    }
    if (wakeupBit & BIT(MENU_PIN))
    {
        wakeUpLong(MENU_PIN, Menu, LongMenu);
        return;
    }
    if (wakeupBit & BIT(DOWN_PIN))
    {
        wakeUpLong(DOWN_PIN, Down, LongDown);
        return;
    }
    if (wakeupBit & BIT(UP_PIN))
    {
        wakeUpLong(UP_PIN, Up, LongUp);
        return;
    }
}

#if DEBUG
void dumpButtons()
{
    if (digitalRead(MENU_PIN) == 1)
    {
        debugLog("Menu button pressed");
    }
    else if (digitalRead(BACK_PIN) == 1)
    {
        debugLog("Back button pressed");
    }
    else if (digitalRead(UP_PIN) == 1)
    {
        debugLog("Up button pressed");
    }
    else if (digitalRead(DOWN_PIN) == 1)
    {
        debugLog("Down button pressed");
    }
}

String getButtonString(buttonState state)
{
    switch (state)
    {
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
        return "None";
    }
}
#endif
