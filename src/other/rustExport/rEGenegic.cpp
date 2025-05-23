#include "rEGeneric.h"

extern "C"
{
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

    extern void rust_panic() {
        assert(0);
    }

    extern void delayRust(int timeMs) {
        delayTask(timeMs);
    }
}