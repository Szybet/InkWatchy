#include "scomTask.h"
#if SCOM_TASK && DEBUG

bool scomChanged = false;

void loopScomTask(void *parameter)
{
    delayTask(3000);
    resetSleepDelay(10000);
    while (true)
    {
        serialWrite.lock();
        // long-back-button:
        // back-button:
        String cmd = Serial.readStringUntil(':');
        if (cmd != "")
        {
            serialWrite.unlock();
            if (cmd.lastIndexOf("button") != -1)
            {
                if (cmd.lastIndexOf("back") != -1)
                {
                    if (cmd.lastIndexOf("long") != -1)
                    {
                        setButton(LongBack);
                    }
                    else
                    {
                        setButton(Back);
                    }
                }
                else if (cmd.lastIndexOf("menu") != -1)
                {
                    if (cmd.lastIndexOf("long") != -1)
                    {
                        setButton(LongMenu);
                    }
                    else
                    {
                        setButton(Menu);
                    }
                }
                else if (cmd.lastIndexOf("up") != -1)
                {
                    if (cmd.lastIndexOf("long") != -1)
                    {
                        setButton(LongUp);
                    }
                    else
                    {
                        setButton(Up);
                    }
                }
                else if (cmd.lastIndexOf("down") != -1)
                {
                    if (cmd.lastIndexOf("long") != -1)
                    {
                        setButton(LongDown);
                    }
                    else
                    {
                        setButton(Down);
                    }
                }
                else if (cmd.lastIndexOf("screen") != -1)
                {
                    scomChanged = true;
                }
            }
            else if (cmd.lastIndexOf("screen") != -1)
            {
                scomChanged = true;
            }
            serialWrite.lock();
        }

        bool scomDid = false;
        if (scomChanged == true)
        {
            scomChanged = false;
            scomDid = true;
            // Latin capital ligature OE
            Serial.write(140);
            for (int i = 0; i < ((display.width() / 8) * display.height()); i++)
            {
                // GxEPD2_Type::WIDTH / 8) * page_height
                String str = String(display._buffer[i]);
                while (str.length() < 3)
                {
                    str = "0" + str;
                }
                Serial.print(str);
            }
        }
        // Print EOF
        Serial.write(26);
        Serial.flush(true);

        serialWrite.unlock();
        resetSleepDelay(1000);
        if (scomDid == true)
        {
            delayTask(450);
        }
        else
        {
            delayTask(225);
        }
    }
}

void initScomTask()
{
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