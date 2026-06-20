#include "blePeripheral.h"

#if BLE_PERIPHERAL && PERIPHERAL_TOWERFALL
#include "rtcMem.h"

// BIH - BLE Input handler
// Towerfall
bool TFrightGo = false;
bool TFleftGo = false;
bool TFUpGo = false;
bool TFDownGo = false;
static bool vibrationSave = false; // Not a good solution but works
static bool shootPressed = false;
static bool jumpPressed = false;
static bool dashPressed = false;
static int64_t backPressedTimer = 0; // ms timer
static int64_t aliveTimer = 0;
#define TF_TILT_DEG 13

void BIHInitTowerFall()
{
    debugLog("Running BIHInitTowerFall");
    TFrightGo = false;
    TFleftGo = false;
    TFUpGo = false;
    TFDownGo = false;
    shootPressed = false;
    jumpPressed = false;
    dashPressed = false;
    backPressedTimer = 0;
    aliveTimer = 0;

    vibrationSave = rM.disableAllVibration;
    rM.disableAllVibration = true;
    turnOffInterrupts();

    init_general_page(5);
    general_page_set_title("Towerfall controller");
    genpage_set_center();
    genpage_add("Tilt to move");
    genpage_add("up button to jump");
    genpage_add("down button to shoot");
    genpage_add("menu button to dash");
    genpage_add("Hold back,up,down to exit");
    general_page_set_main();
    slint_loop();

    initAcc();
    delayTask(100);
}

void BIHLoopTowerFall()
{
    delayTask(30);
    // debugLog("Running BIHLoopTowerFall");
    Accel acc;
    int16_t degX;
    int16_t degY;
    if (rM.SBMA.getAccel(&acc))
    {
        degX = getAxisDegrees(acc.x, acc.y, acc.z);
        degY = getAxisDegrees(acc.y, acc.x, acc.z);

        debugLog("degX: " + String(degX) + " degY: " + String(degY));
        if (!TFrightGo)
        {
            if (degX > TF_TILT_DEG)
            {
                debugLog("Pressing right go");
                bleKeyboard->press(KEY_RIGHT_ARROW);
                TFrightGo = true;
            }
        }
        else
        {
            if (degX < TF_TILT_DEG)
            {
                debugLog("Releasing right go");
                bleKeyboard->release(KEY_RIGHT_ARROW);
                TFrightGo = false;
            }
        }

        if (!TFleftGo)
        {
            if (degX < TF_TILT_DEG * -1)
            {
                debugLog("Pressing right go");
                bleKeyboard->press(KEY_LEFT_ARROW);
                TFleftGo = true;
            }
        }
        else
        {
            if (degX > TF_TILT_DEG * -1)
            {
                debugLog("Releasing right go");
                bleKeyboard->release(KEY_LEFT_ARROW);
                TFleftGo = false;
            }
        }

        if (!TFUpGo)
        {
            if (degY < TF_TILT_DEG * -1)
            {
                debugLog("Pressing up go");
                bleKeyboard->press(KEY_UP_ARROW);
                TFUpGo = true;
            }
        }
        else
        {
            if (degY > TF_TILT_DEG * -1)
            {
                debugLog("Releasing up go");
                bleKeyboard->release(KEY_UP_ARROW);
                TFUpGo = false;
            }
        }

        if (!TFDownGo)
        {
            if (degY > TF_TILT_DEG)
            {
                debugLog("Pressing down go");
                bleKeyboard->press(KEY_DOWN_ARROW);
                TFDownGo = true;
            }
        }
        else
        {
            if (degY < TF_TILT_DEG)
            {
                debugLog("Releasing down go");
                bleKeyboard->release(KEY_DOWN_ARROW);
                TFDownGo = false;
            }
        }
    }
    else
    {
        debugLog("ACC failed for towerfall");
    }

    delayTask(30);
    buttonStates btns = readButtons();
    if (btns.down == true && shootPressed == false)
    {
        bleKeyboard->press('x');
        shootPressed = true;
    }
    else if (btns.down == false && shootPressed == true)
    {
        bleKeyboard->release('x');
        shootPressed = false;
    }

    if (btns.up == true && jumpPressed == false)
    {
        bleKeyboard->press('c');
        jumpPressed = true;
    }
    else if (btns.up == false && jumpPressed == true)
    {
        bleKeyboard->release('c');
        jumpPressed = false;
    }

    if (btns.menu == true && dashPressed == false)
    {
        bleKeyboard->press(KEY_LEFT_SHIFT);
        dashPressed = true;
    }
    else if (btns.menu == false && dashPressed == true)
    {
        bleKeyboard->release(KEY_LEFT_SHIFT);
        dashPressed = false;
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
                debugLog("Exiting towerfall");
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

void BIHExitTowerFall()
{
    debugLog("Running BIHExitTowerFall");
    slintExit();
    rM.disableAllVibration = vibrationSave;
    turnOnInterrupts();
}

void selectTowerFall()
{
    bleInputHandlerInit = BIHInitTowerFall;
    bleInputHandlerLoop = BIHLoopTowerFall;
    bleInputHandlerExit = BIHExitTowerFall;
    setButton(Back);
}
#endif
