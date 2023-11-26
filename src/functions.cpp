#include "functions.h"

void logFunction(String file, int line, String func, String message) {
    Serial.println(file + ":" + String(line) + " " + func + ": " + message);
}
