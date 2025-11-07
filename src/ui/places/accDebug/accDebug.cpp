#include "accDebug.h"
#include "rtcMem.h"

#if DEBUG_MENUS == 1

bool workingAcc = false;

bool runAccelStatus = false;
uint8_t lineRunAccelStatus = 0;

int16_t accX = 0;
uint8_t lineAccX;
int16_t accY = 0;
uint8_t lineAccY;
int16_t accZ = 0;
uint8_t lineAccZ;

#if BMA_VERSION == 530 || BMA_VERSION == 456
int16_t accXPure = 0;
uint8_t lineAccXPure;
int16_t accYPure = 0;
uint8_t lineAccYPure;
int16_t accZPure = 0;
uint8_t lineAccZPure;
#endif

uint16_t stepsAccDebug = 0;
uint8_t lineAccSteps;

int16_t errorCode = 0;
uint8_t lineErrorCode;
int16_t statusCode = 0;
uint8_t lineStatusCode;

bool is3DOn = false;

void turn3D()
{
#if SCREEN_ROTATION != 0
  if (is3DOn == true)
  {
    dis->setRotation(SCREEN_ROTATION);
  }
  else
  {
    dis->setRotation(0);
  }
#endif
  is3DOn = !is3DOn;
}

void initAccDebug()
{
#if ACC_ENABLED
  initAcc();
#endif

  is3DOn = false;

  init_general_page(5);
  general_page_set_title(DEBUG_MENU_ACC);

  String accDevice = DEBUG_ACC_IC;
#if !ACC_ENABLED
  accDevice += DEBUG_ACC_DISABLED;
#else
  accDevice += "BMA" + String(BMA_VERSION);
#endif

  genpage_add(accDevice.c_str());

#if ACC_ENABLED
  initAcc();
  if (rM.initedAcc == false)
  {
    workingAcc = false;
    genpage_add(DEBUG_ACC_FAILED_INIT);
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
      lineRunAccelStatus = genpage_add(DEBUG_ACC_DAMAGED);
    }
    else
    {
      workingAcc = true;
      lineRunAccelStatus = genpage_add(DEBUG_ACC_WORKING);
    }
  }

  if (workingAcc == true)
  {
    delayTask(50);
    rM.SBMA.enableAccel();
    Accel acc;
    runAccelStatus = rM.SBMA.getAccel(&acc);

    GeneralPageButton button = GeneralPageButton{DEBUG_ACC_CLICK_3D, turn3D};
    general_page_set_buttons(&button, 1);

    lineAccX = genpage_add(String(DEBUG_ACC_X + String(acc.x)).c_str());
    accX = acc.x;
    lineAccY = genpage_add(String(DEBUG_ACC_Y + String(acc.y)).c_str());
    accY = acc.y;
    lineAccZ = genpage_add(String(DEBUG_ACC_Z + String(acc.z)).c_str());
    accZ = acc.z;

    stepsAccDebug = getSteps();
    lineAccSteps = genpage_add(String(DEBUG_ACC_STEPS + String(stepsAccDebug)).c_str());

    errorCode = rM.SBMA.getErrorCode();
    lineErrorCode = genpage_add(String(DEBUG_ACC_ERROR_CODE + String(errorCode)).c_str());
    statusCode = rM.SBMA.getStatus();
    lineStatusCode = genpage_add(String(DEBUG_ACC_STATUS_CODE + String(statusCode)).c_str());

#if BMA_VERSION == 530 || BMA_VERSION == 456
    Accel accPure;
    rM.SBMA.getAccelPure(&accPure);
    lineAccXPure = genpage_add(String(DEBUG_ACC_PURE_X + String(accPure.x)).c_str());
    accXPure = accPure.x;
    lineAccYPure = genpage_add(String(DEBUG_ACC_PURE_Y + String(accPure.y)).c_str());
    accYPure = accPure.y;
    lineAccZPure = genpage_add(String(DEBUG_ACC_PURE_Z + String(accPure.z)).c_str());
    accZPure = accPure.z;
#endif
  }

#endif

  general_page_set_main();
}

int accDataTime = 0;
void loopAccDebug()
{
#if ACC_ENABLED
  if (is3DOn == false)
  {
    // debugLog("damagedAcc: " + BOOL_STR(rM.SBMA.damagedAcc));
    if (workingAcc == true)
    {
      if (genpage_is_menu() == false)
      {
        uint16_t tmpSteps = stepsAccDebug;
        if (millisBetter() - accDataTime > 500)
        {
          accDataTime = millisBetter();
          // It's moved here to get the status before requesting acceleration
          int16_t tmpStatusCode = rM.SBMA.getStatus();

          Accel acc;
          bool runAccelStatusTmp = rM.SBMA.getAccel(&acc);

          // Weird fallback
          /*
          if (runAccelStatusTmp == true && acc.x == 0 && acc.y == 0 && acc.z == 0)
          {
            runAccelStatusTmp = false;
            debugLog("Weird acc workaround fix");
            rM.SBMA.enableAccel();
            rM.SBMA.enableStepCount();
          }
          */

          if (runAccelStatusTmp != runAccelStatus)
          {
            runAccelStatus = runAccelStatusTmp;
            if (runAccelStatus == true)
            {
              genpage_change(String(DEBUG_ACC_WORKING).c_str(), lineRunAccelStatus);
            }
            else
            {
              genpage_change(String(DEBUG_ACC_DAMAGED).c_str(), lineRunAccelStatus);
            }
          }
          if (acc.x != accX)
          {
            genpage_change(String(DEBUG_ACC_X + String(acc.x)).c_str(), lineAccX);
            accX = acc.x;
            // debugLog("Acc x changed");
          }
          if (acc.y != accY)
          {
            genpage_change(String(DEBUG_ACC_Y + String(acc.y)).c_str(), lineAccY);
            accY = acc.y;
            // debugLog("Acc y changed");
          }
          if (acc.z != accZ)
          {
            genpage_change(String(DEBUG_ACC_Z + String(acc.z)).c_str(), lineAccZ);
            accZ = acc.z;
            // debugLog("Acc z changed");
          }

          uint16_t tmpSteps = getSteps();
          if (tmpSteps != stepsAccDebug)
          {
            genpage_change(String(DEBUG_ACC_STEPS + String(tmpSteps)).c_str(), lineAccSteps);
            stepsAccDebug = tmpSteps;
            // debugLog("Steps changed");
          }

          int16_t tmpErrorCode = rM.SBMA.getErrorCode();
          if (tmpErrorCode != errorCode)
          {
            genpage_change(String(DEBUG_ACC_ERROR_CODE + String(tmpErrorCode)).c_str(), lineErrorCode);
            errorCode = tmpErrorCode;
          }

          if (tmpStatusCode != statusCode)
          {
            genpage_change(String(DEBUG_ACC_STATUS_CODE + String(tmpStatusCode)).c_str(), lineStatusCode);
            statusCode = tmpStatusCode;
          }

#if BMA_VERSION == 530 || BMA_VERSION == 456
          Accel accPure;
          rM.SBMA.getAccelPure(&accPure);
          if (accPure.x != accXPure)
          {
            genpage_change(String(DEBUG_ACC_PURE_X + String(accPure.x)).c_str(), lineAccXPure);
            accXPure = accPure.x;
          }
          if (accPure.y != accYPure)
          {
            genpage_change(String(DEBUG_ACC_PURE_Y + String(accPure.y)).c_str(), lineAccYPure);
            accYPure = accPure.y;
          }
          if (accPure.z != accZPure)
          {
            genpage_change(String(DEBUG_ACC_PURE_Z + String(accPure.z)).c_str(), lineAccZPure);
            accZPure = accPure.z;
          }
#endif
          general_page_set_main();
        }
      }
    }
  }
  else
  {
    if (useButton() == Menu)
    {
      turn3D();
      return;
    }
    loopAcc3D();
    disUp();
  }

  resetSleepDelay();
#endif
  if (is3DOn == false)
  {
    // Here because to show IC disabled too
    slint_loop();
  }
}

void exitAccDebug()
{
#if SCREEN_ROTATION != 0
  dis->setRotation(SCREEN_ROTATION);
#endif
  slintExit();
}
#endif
