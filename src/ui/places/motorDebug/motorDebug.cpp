#include "motorDebug.h"
#include "rtcMem.h"

#if DEBUG || DEBUG_MENUS

uint8_t lineVibrationsDisabled;
bool oldDisableAllVibration = false;

uint8_t lineVibrationPower;
int vibrationPowerToSet = VIBRATION_POWER;
int oldVibrationPower = VIBRATION_POWER;
#define VIBRATION_POWER_STEP 5

uint8_t lineVibrationTime;
int vibrationTimeToSet = 50;
int oldVibratiomTime = 50;
#define VIBRATION_TIME_STEP 10

uint8_t lineBatteryVoltage;
float oldBatteryVoltage;

#define SET_POWER 0
#define SET_TIME 1
uint8_t motorSetChanger = SET_POWER;

String vibrationDisabledStr()
{
    oldDisableAllVibration = rM.disableAllVibration;
    return String(DEBUG_MOTOR_VIB_DIS + String(BOOL_STR(rM.disableAllVibration)));
}

String vibrationPowerStr()
{
    oldVibrationPower = vibrationPowerToSet;
    return String(DEBUG_MOTOR_VIB_POWER + String(vibrationPowerToSet));
}

String vibrationTimeStr()
{
    oldVibratiomTime = vibrationTimeToSet;
    return String(DEBUG_MOTOR_VIB_TIME + String(vibrationTimeToSet));
}

String batteryVoltage()
{
    oldBatteryVoltage = rM.bat.curV;
    return String(DEBUG_MOTOR_BATT_V + String(rM.bat.curV));
}

void setMotorSetPower()
{
    motorSetChanger = SET_POWER;
}

void setMotorSetTime()
{
    motorSetChanger = SET_TIME;
}

void initMotorDebug()
{
    init_general_page(50);
    general_page_set_title(DEBUG_MENU_MOTOR);
    rustLimitButtons = true;

    GeneralPageButton button[] = {GeneralPageButton{DEBUG_MOTOR_SET_POWER, setMotorSetPower}, GeneralPageButton{DEBUG_MOTOR_SET_TIME, setMotorSetTime}};
    general_page_set_buttons(button, 2);

    lineVibrationsDisabled = genpage_add(vibrationDisabledStr().c_str());

    lineBatteryVoltage = genpage_add(batteryVoltage().c_str());

    lineVibrationPower = genpage_add(vibrationPowerStr().c_str());

    lineVibrationTime = genpage_add(vibrationTimeStr().c_str());

    resetSleepDelay(SLEEP_EVERY_MS);
    general_page_set_main();
}

void loopMotorDebug()
{
    if (genpage_is_menu() == false)
    {

        loopBattery();

        if (rM.disableAllVibration != oldDisableAllVibration)
        {
            genpage_change(vibrationDisabledStr().c_str(), lineVibrationsDisabled);
        }

        if (rM.bat.curV != oldBatteryVoltage)
        {
            genpage_change(batteryVoltage().c_str(), lineBatteryVoltage);
        }

        if (vibrationPowerToSet != oldVibrationPower)
        {
            genpage_change(vibrationPowerStr().c_str(), lineVibrationPower);
        }

        if (vibrationTimeToSet != oldVibratiomTime)
        {
            genpage_change(vibrationTimeStr().c_str(), lineVibrationTime);
        }

        buttonState btn = getButtonsRust();
        switch (btn)
        {
        case Menu:
        {
            vibrateMotor(vibrationTimeToSet, vibrationPowerToSet);
            break;
        }
        case Up:
        {
            if (motorSetChanger == SET_POWER)
            {
                vibrationPowerToSet = vibrationPowerToSet + VIBRATION_POWER_STEP;
                checkMaxMin(&vibrationPowerToSet, 255, 0);
            }
            else if (motorSetChanger == SET_TIME)
            {
                vibrationTimeToSet = vibrationTimeToSet + VIBRATION_TIME_STEP;
                checkMaxMin(&vibrationTimeToSet, 1000, 0);
            }
            break;
        }
        case Down:
        {
            if (motorSetChanger == SET_POWER)
            {
                vibrationPowerToSet = vibrationPowerToSet - VIBRATION_POWER_STEP;
                checkMaxMin(&vibrationPowerToSet, 255, 0);
            }
            else if (motorSetChanger == SET_TIME)
            {
                vibrationTimeToSet = vibrationTimeToSet - VIBRATION_TIME_STEP;
                checkMaxMin(&vibrationTimeToSet, 1000, 0);
            }
            break;
        }
        }
    }

    general_page_set_main();
    slint_loop();
}

#endif