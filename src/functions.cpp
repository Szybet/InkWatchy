#include "functions.h"

void logFunction(String file, int line, String func, String message) {
    Serial.println(file + ":" + String(line) + " " + func + ": " + message);
}

// Check if a function contains a character that has a line below like... g p q j
bool containsBelowChar(String str) {
    if(str.indexOf("g") > 0 || str.indexOf("p") > 0 || str.indexOf("q") > 0 || str.indexOf("j") > 0) {
        return true;
    } else {
        return false;
    }
}

