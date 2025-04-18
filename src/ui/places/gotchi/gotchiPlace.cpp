#include "gotchiPlace.h"

#if GOTCHI

#include "emulator/cpu.h"
GFXcanvas1 *gotchiBuffOutput;

void initGotchi()
{
    gotchiButtonsEx = {0, 0, 0};
    gotchiBuffOutput = new GFXcanvas1(200, 200, true);
    gotchiBuffOutput->fillScreen(GxEPD_WHITE);

    bufSize buff = fsGetBlob("tomagotchiRom.bin", "/other/");
    debugLog("size is: " + String(buff.size));
    if(buff.size != 9216) {
        debugLog("Failed to load rom");
        delayTask(1000);
        assert(false);
    }
    romData = buff.buf;

    startGotchiTask();
}

#define BUFF_SIZE 5000 // ((200 + 7) / 8) * 200;

void loopGotchi()
{
    // debugLog("Looping gotchi");
    {
        gotchiBuffMutex.lock();
        uint8_t *sourceBuff = gotchiBuff->getBuffer();
        uint8_t *destinationBuff = gotchiBuffOutput->getBuffer();
        memcpy(destinationBuff, sourceBuff, BUFF_SIZE);
        gotchiBuffMutex.unlock();
    }

    {
        uint8_t *sourceBuff = gotchiBuffOutput->getBuffer();
        uint8_t *destinationBuff = dis->_buffer;
        if (memcmp(sourceBuff, destinationBuff, BUFF_SIZE) == 0)
        {
            debugLog("The buffers are equal, not doing anything");
        }
        else
        {
            debugLog("The buffers are not equal, showing it");
            memcpy(destinationBuff, sourceBuff, BUFF_SIZE);
            dUChange = true;
        }
    }

    buttonState btn = useButton();
    switch (btn)
    {
    case Up:
    {
        gotchiButtonsEx.right = gotchiButtonsEx.right + BUTTON_TICKS_NEEDED;
        break;
    }
    case Down:
    {
        gotchiButtonsEx.left = gotchiButtonsEx.left + BUTTON_TICKS_NEEDED;
        break;
    }
    case Menu:
    {
        gotchiButtonsEx.middle = gotchiButtonsEx.middle + BUTTON_TICKS_NEEDED;
        break;
    }
    }
    
    resetSleepDelay();
    disUp();
}

void exitGotchi() {
    endGotchiTask();
    delete gotchiBuffOutput;
    free(romData);
}

#endif