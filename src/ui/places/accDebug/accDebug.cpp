#include "accDebug.h"
#include "rtcMem.h"

#if DEBUG_MENUS == 1

#define CURSOR_X 1
#define ES "        "

bool workingAcc = false;

int16_t accX = 0;
uint16_t accXHeight = 0;
int16_t accY = 0;
uint16_t accYHeight = 0;
int16_t accZ = 0;
uint16_t accZHeight = 0;

#if BMA_VERSION == 530 || BMA_VERSION == 456
int16_t accXPure = 0;
uint16_t accXHeightPure = 0;
int16_t accYPure = 0;
uint16_t accYHeightPure = 0;
int16_t accZPure = 0;
uint16_t accZHeightPure = 0;
#endif

uint16_t steps = 0;
uint16_t stepsHeight = 0;

void initAccDebug()
{
#if ACC_ENABLED
    initAcc();
#endif
    dis->setTextWrap(false);

    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = "Debug Menu: Acc";
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    maxHeight = h;
    uint16_t currentHeight = maxHeight;
    dis->setCursor(CURSOR_X, currentHeight - 3);
    dis->print(menuName);

    dis->fillRect(0, currentHeight, dis->width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;

    String accDevice;
#if !ACC_ENABLED
    accDevice = "disabled";
#else
    accDevice = "BMA" + String(BMA_VERSION);
#endif

    writeLine("IC: " + accDevice, CURSOR_X, &currentHeight);

#if ACC_ENABLED
    initAcc();
    if (rM.initedAcc == false)
    {
        workingAcc = false;
        writeLine("Failed to init Acc", CURSOR_X, &currentHeight);
    }

    if (rM.initedAcc == true)
    {
        // To cause an error if something goes wrong (selftest doesn't work now, this does)
        rM.SBMA.enableAccel();
        Accel acc;
        rM.SBMA.getAccel(&acc);
        rM.SBMA.disableAccel();

        if (rM.SBMA.damagedAcc == true)
        {
            workingAcc = false;
            writeLine("Acc is damaged", CURSOR_X, &currentHeight);
        }
        else
        {
            workingAcc = true;
            writeLine("Acc is working", CURSOR_X, &currentHeight);
        }
    }

    if (workingAcc == true)
    {
        rM.SBMA.enableAccel();
        Accel acc;
        rM.SBMA.getAccel(&acc);

        writeLine("Accel X: " + String(acc.x), CURSOR_X, &currentHeight);
        accX = acc.x;
        accXHeight = currentHeight - maxHeight;
        writeLine("Accel Y: " + String(acc.y), CURSOR_X, &currentHeight);
        accY = acc.y;
        accYHeight = currentHeight - maxHeight;
        writeLine("Accel Z: " + String(acc.z), CURSOR_X, &currentHeight);
        accZ = acc.z;
        accZHeight = currentHeight - maxHeight;

        steps = getSteps();
        writeLine("Steps: " + String(steps), CURSOR_X, &currentHeight);
        stepsHeight = currentHeight - maxHeight;

#if BMA_VERSION == 530 || BMA_VERSION == 456
        Accel accPure;
        rM.SBMA.getAccelPure(&accPure);
        writeLine("Accel pure X: " + String(accPure.x), CURSOR_X, &currentHeight);
        accXPure = accPure.x;
        accXHeightPure = currentHeight - maxHeight;
        writeLine("Accel pure Y: " + String(accPure.y), CURSOR_X, &currentHeight);
        accYPure = accPure.y;
        accYHeightPure = currentHeight - maxHeight;
        writeLine("Accel pure Z: " + String(accPure.z), CURSOR_X, &currentHeight);
        accZPure = accPure.z;
        accZHeightPure = currentHeight - maxHeight;
#endif
    }

#endif
    resetSleepDelay();
    disUp(true);
}

void loopAccDebug()
{
#if ACC_ENABLED
    // debugLog("damagedAcc: " + BOOL_STR(rM.SBMA.damagedAcc));
    if (workingAcc == true)
    {
        Accel acc;
        rM.SBMA.getAccel(&acc);
        if (acc.x != accX)
        {
            writeTextReplaceBack("Accel X: " + String(acc.x) + ES, CURSOR_X, accXHeight);
            accX = acc.x;
            dUChange = true;
            // debugLog("Acc x changed");
        }
        if (acc.y != accY)
        {
            writeTextReplaceBack("Accel Y: " + String(acc.y) + ES, CURSOR_X, accYHeight);
            accY = acc.y;
            dUChange = true;
            // debugLog("Acc y changed");
        }
        if (acc.z != accZ)
        {
            writeTextReplaceBack("Accel Z: " + String(acc.z) + ES, CURSOR_X, accZHeight);
            accZ = acc.z;
            dUChange = true;
            // debugLog("Acc z changed");
        }

        uint16_t tmpSteps = getSteps();
        if (tmpSteps != steps)
        {
            writeTextReplaceBack("Steps: " + String(tmpSteps) + "    ", CURSOR_X, stepsHeight);
            steps = tmpSteps;
            dUChange = true;
            // debugLog("Steps changed");
        }

#if BMA_VERSION == 530 || BMA_VERSION == 456
        Accel accPure;
        rM.SBMA.getAccelPure(&accPure);
        if (accPure.x != accXPure)
        {
            writeTextReplaceBack("Accel pure X: " + String(accPure.x) + ES, CURSOR_X, accXHeightPure);
            accXPure = accPure.x;
            dUChange = true;
        }
        if (accPure.y != accYPure)
        {
            writeTextReplaceBack("Accel pure Y: " + String(accPure.y) + ES, CURSOR_X, accYHeightPure);
            accYPure = accPure.y;
            dUChange = true;
        }
        if (accPure.z != accZPure)
        {
            writeTextReplaceBack("Accel pure Z: " + String(accPure.z) + ES, CURSOR_X, accZHeightPure);
            accZPure = accPure.z;
            dUChange = true;
        }
#endif
    }

    resetSleepDelay();
#endif
    useButtonBlank();
    disUp();
}

void exitAccDebug()
{
#if ACC_ENABLED
    rM.SBMA.disableAccel();
#endif
}
#endif