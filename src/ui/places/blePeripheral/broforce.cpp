#include "blePeripheral.h"

#if BLE_PERIPHERAL && PERIPHERAL_BROFORCE
#include "rtcMem.h"

// BIH - BLE Input handler
// Broforce
bool BFrightGo = false;
bool BFleftGo = false;
bool BFUpGo = false;
bool BFDownGo = false;
static bool vibrationSave = false; // Not a good solution but works
static bool shootPressed = false;
static bool specialPressed = false;
static bool meelePressed = false;
static int64_t backPressedTimer = 0; // ms timer
static int64_t aliveTimer = 0;
#define BF_TILT_DEG 13

void BIHInitBroforce()
{
    debugLog("Running BIHInitBroforce");
    BFrightGo = false;
    BFleftGo = false;
    BFUpGo = false;
    BFDownGo = false;
    shootPressed = false;
    specialPressed = false;
    meelePressed = false;
    backPressedTimer = 0;
    aliveTimer = 0;

    vibrationSave = rM.disableAllVibration;
    rM.disableAllVibration = true;
    turnOffInterrupts();

    init_general_page(5);
    general_page_set_title("Broforce controller");
    genpage_set_center();
    genpage_add("Tilt to move");
    genpage_add("up button to special");
    genpage_add("down button to shoot");
    genpage_add("menu button to dash");
    genpage_add("Hold back,up,down to exit");
    general_page_set_main();
    slint_loop();

    initAcc();
    delayTask(100);
}

void BIHLoopBroforce()
{
    delayTask(30);
    // debugLog("Running BIHLoopBroforce");
    Accel acc;
    int16_t degX;
    int16_t degY;
    if (rM.SBMA.getAccel(&acc))
    {
        degX = getAxisDegrees(acc.x, acc.y, acc.z);
        degY = getAxisDegrees(acc.y, acc.x, acc.z);

        debugLog("degX: " + String(degX) + " degY: " + String(degY));
        if (!BFrightGo)
        {
            if (degX > BF_TILT_DEG)
            {
                debugLog("Pressing right go");
                bleKeyboard->press(KEY_RIGHT_ARROW);
                BFrightGo = true;
            }
        }
        else
        {
            if (degX < BF_TILT_DEG)
            {
                debugLog("Releasing right go");
                bleKeyboard->release(KEY_RIGHT_ARROW);
                BFrightGo = false;
            }
        }

        if (!BFleftGo)
        {
            if (degX < BF_TILT_DEG * -1)
            {
                debugLog("Releasing left go");
                bleKeyboard->press(KEY_LEFT_ARROW);
                BFleftGo = true;
            }
        }
        else
        {
            if (degX > BF_TILT_DEG * -1)
            {
                debugLog("Releasing left go");
                bleKeyboard->release(KEY_LEFT_ARROW);
                BFleftGo = false;
            }
        }

        if (!BFUpGo)
        {
            if (degY < BF_TILT_DEG * -1)
            {
                debugLog("Pressing up go");
                bleKeyboard->press(KEY_UP_ARROW);
                BFUpGo = true;
            }
        }
        else
        {
            if (degY > BF_TILT_DEG * -1)
            {
                debugLog("Releasing up go");
                bleKeyboard->release(KEY_UP_ARROW);
                BFUpGo = false;
            }
        }

        if (!BFDownGo)
        {
            if (degY > BF_TILT_DEG)
            {
                debugLog("Pressing down go");
                bleKeyboard->press(KEY_DOWN_ARROW);
                BFDownGo = true;
            }
        }
        else
        {
            if (degY < BF_TILT_DEG)
            {
                debugLog("Releasing down go");
                bleKeyboard->release(KEY_DOWN_ARROW);
                BFDownGo = false;
            }
        }
    }
    else
    {
        debugLog("ACC failed for broforce");
    }

    delayTask(30);
    buttonStates btns = readButtons();
    if (btns.down == true && shootPressed == false)
    {
        bleKeyboard->press('z');
        shootPressed = true;
    }
    else if (btns.down == false && shootPressed == true)
    {
        bleKeyboard->release('z');
        shootPressed = false;
    }

    if (btns.up == true && specialPressed == false)
    {
        bleKeyboard->press('x');
        specialPressed = true;
    }
    else if (btns.up == false && specialPressed == true)
    {
        bleKeyboard->release('x');
        specialPressed = false;
    }

    if (btns.menu == true && meelePressed == false)
    {
        bleKeyboard->press('c');
        meelePressed = true;
    }
    else if (btns.menu == false && meelePressed == true)
    {
        bleKeyboard->release('c');
        meelePressed = false;
    }

    if (btns.back && btns.up && btns.down)
    {
        if (backPressedTimer == 0)
        {
            backPressedTimer = millisBetter();
        }
        else
        {
            if (millisBetter() - backPressedTimer > 7000)
            {
                debugLog("Exiting broforce");
                setButton(Back);
            }
        }
    }
    else
    {
        if (backPressedTimer != 0)
        {
            backPressedTimer = 0;
        }
    }

    if (millisBetter() - aliveTimer > 10000)
    {
        debugLog("Writing alive key");
        bleKeyboard->write(KEY_F20);
        aliveTimer = millisBetter();
    }

    resetSleepDelay();
}

void BIHExitBroforce()
{
    debugLog("Running BIHExitBroforce");
    slintExit();
    rM.disableAllVibration = vibrationSave;
    turnOnInterrupts();
}

void selectBroforce()
{
    bleInputHandlerInit = BIHInitBroforce;
    bleInputHandlerLoop = BIHLoopBroforce;
    bleInputHandlerExit = BIHExitBroforce;
    setButton(Back);
}
#endif
