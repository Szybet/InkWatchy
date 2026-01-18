#include "notes.h"

#if NOTES_APP
void initNotesApp()
{
    initBleHost(new BleKeyboardCallbacks());
}

void loopNotesApp()
{
    delayTask(400);
    resetSleepDelay();
}

void exitNotesApp()
{

}
#endif
