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

    extern void drawPixel(int16_t x, int16_t y, uint16_t color) {
        dis->drawPixel(x, y, color);
    }

    extern void updateScreen(bool reallyUpdate, bool ignoreCounter, bool ignoreSleep) {
        disUp(reallyUpdate, ignoreCounter, ignoreSleep);
    }

    extern uint32_t rustMicros() {
        return micros();
    }

    extern uint32_t rustButtons() {
        buttonState button = useButton();
        if(button == buttonState::None) {
            return RUST_BUTTON_NONE;
        }
        if(button == buttonState::Up) {
            return RUST_BUTTON_UP;
        }
        if(button == buttonState::Down) {
            return RUST_BUTTON_DOWN;
        }
        if(button == buttonState::Menu) {
            return RUST_BUTTON_MENU;
        }
        if(button == buttonState::LongMenu) {
            return RUST_BUTTON_MENU_LONG;
        }
        return RUST_BUTTON_NONE;
    }

    extern void rustResetDelay(){
        resetSleepDelay();
    }

    extern void rustSetCpuSpeed(int speed) {
        if(cpuSpeed::minimalSpeed == 0) {
            setCpuSpeed(cpuSpeed::minimalSpeed);
        }
        if(cpuSpeed::normalSpeed == 1) {
            setCpuSpeed(cpuSpeed::normalSpeed);
        }
        if(cpuSpeed::maxSpeed == 2) {
            setCpuSpeed(cpuSpeed::maxSpeed);
        }
    }

    extern int rustGetCpuSpeed() {
        cpuSpeed speed = getCpuSpeed();
        if(speed == minimalSpeed) {
            return 0;
        }
        if(speed == normalSpeed) {
            return 1;
        }
        if(speed == maxSpeed) {
            return 2;
        }
        return -1;
    }
}