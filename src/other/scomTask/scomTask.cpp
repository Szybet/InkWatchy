#include "scomTask.h"
#if SCOM_TASK && DEBUG

void loopScomTask(void *parameter) {
    delayTask(10000);
    while(true) {
        serialWrite.lock();
        // Latin capital ligature OE
        Serial.write(140);
        for(int i = 0; i < ((display.width() / 8) * display.height()); i++) {
            // GxEPD2_Type::WIDTH / 8) * page_height
            String str = String(display._buffer[i]);
            while(str.length() < 3) {
                str = "0" + str;
            }
            Serial.print(str);
        }
        // Print EOF
        Serial.write(26);
        Serial.flush();
        serialWrite.unlock();
        delayTask(1000);
        resetSleepDelay(10000);
    }
}

void initScomTask() {
    debugLog("- done");
    xTaskCreate(
        loopScomTask,
        "scomTask",
        3000,
        NULL,
        0,
        nullptr);
}
#endif