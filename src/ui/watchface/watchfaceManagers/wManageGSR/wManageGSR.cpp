#include "wManageGSR.h"
#include "rtcMem.h"

#if GSR_WATCHFACES

void wManageGsrLaunch(WatchyGSR* gsr, bool init) {
    if (useButton() == Menu)
    {
        resetSleepDelay();
        generalSwitch(mainMenu);
    } else {
        setSleepDelay(0);
        gsr->InsertDrawWatchStyle(0);
    }
    disUp(true, false, true);
}

#endif