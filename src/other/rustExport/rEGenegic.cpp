#include "rEGeneric.h"

extern "C" {
    extern const char *rust_ink_test(void) {
        static char msg[] = "Hello from Ink, rust works!";
        return msg;
    }
}