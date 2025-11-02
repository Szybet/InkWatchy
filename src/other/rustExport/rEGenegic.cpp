#include "rEGeneric.h"

extern "C"
{
#if DEBUG
    extern const char *rust_ink_test(void)
    {
        static char msg[] = "Hello from Ink, rust works!";
        return msg;
    }

    extern void log_function_c(
        const char *file, int line, const char *func, const char *message)
    {
#if DEBUG
        logFunction(String(file), line, String(func), String("\033[31mRust: \033[0m") + String(message));
#endif
    }
#endif

    extern void rustPanic()
    {
        assert(0);
    }

    extern void delayRust(int timeMs)
    {
        delayTask(timeMs);
    }

    extern void updateScreen(bool reallyUpdate, bool ignoreCounter, bool ignoreSleep)
    {
        disUp(reallyUpdate, ignoreCounter, ignoreSleep);
    }

    extern uint32_t rustMicros()
    {
        return micros();
    }

    extern uint32_t rustButtons()
    {
        buttonState button;
        if (rustLimitButtons == false)
        {
            button = useButton();
        }
        else
        {
            buttMut.lock();
            // debugLog("useButtonBack state: " + getButtonString(buttonPressed));
            if (buttonPressed == LongUp || buttonPressed == LongDown || buttonPressed == LongMenu)
            {
                button = buttonPressed;
                buttonPressed = None;
            }
            buttMut.unlock();
        }

        switch (button)
        {
        case buttonState::None:
            return RUST_BUTTON_NONE;
        case buttonState::Up:
            return RUST_BUTTON_UP;
        case buttonState::Down:
            return RUST_BUTTON_DOWN;
        case buttonState::Menu:
            return RUST_BUTTON_MENU;
        case buttonState::LongUp:
            return RUST_BUTTON_UP_LONG;
        case buttonState::LongDown:
            return RUST_BUTTON_DOWN_LONG;
        case buttonState::LongMenu:
            return RUST_BUTTON_MENU_LONG;
        default:
            return RUST_BUTTON_NONE;
        }
    }

    extern void rustResetDelay()
    {
        resetSleepDelay();
    }

    extern void rustSetCpuSpeed(int speed)
    {
        if (cpuSpeed::minimalSpeed == speed)
        {
            setCpuSpeed(cpuSpeed::minimalSpeed);
            return;
        }
        if (cpuSpeed::normalSpeed == speed)
        {
            setCpuSpeed(cpuSpeed::normalSpeed);
            return;
        }
        if (cpuSpeed::maxSpeed == speed)
        {
            setCpuSpeed(cpuSpeed::maxSpeed);
            return;
        }
    }

    extern int rustGetCpuSpeed()
    {
        cpuSpeed speed = getCpuSpeed();
        if (speed == minimalSpeed)
        {
            return 0;
        }
        if (speed == normalSpeed)
        {
            return 1;
        }
        if (speed == maxSpeed)
        {
            return 2;
        }
        return -1;
    }

    extern void rustCleanMemory()
    {
        cleanAllMemory();
    }

    extern const uint16_t *getColorBlack()
    {
        return &SCBlack;
    }

    extern const uint16_t *getColorWhite()
    {
        return &SCWhite;
    }

    extern uint8_t *getScreenBuffer()
    {
        return dis->_buffer;
    }

    extern uint8_t getRotation()
    {
        return dis->getRotation();
    }
}
