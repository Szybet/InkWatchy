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
}