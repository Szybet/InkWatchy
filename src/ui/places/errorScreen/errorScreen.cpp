#include "errorScreen.h"
#include "rtcMem.h"

String errorScreenStr = "";

void showErrorOnScreen(String err)
{
    if (rM.currentPlace != errorDialog &&
        errorScreenStr != err)
    {
        debugLog("Showing ERROR on screen: " + err);
        errorScreenStr = err;

        init_general_page(1);
        general_page_set_title("Error");
        genpage_set_center();
        genpage_add(err.c_str());
        genpage_add("");
        genpage_add("If resolved, click back to exit");
        general_page_set_main();

        generalSwitch(errorDialog);
    }
    else
    {
        debugLog("Error already shown: " + err);
    }
}

IWColors currentErrorColor = IWColors::IwRed;
uint32_t screenErrorMillis = 0;

void loopErrorScreen()
{
    if (millisBetter() - screenErrorMillis > 700)
    {
        screenErrorMillis = millisBetter();
        debugLog("Erroring: " + errorScreenStr);
        vibrateMotor();
        setRgb(currentErrorColor);
        if (currentErrorColor == IWColors::IwRed)
        {
            currentErrorColor = IWColors::IwNone;
        }
        else
        {
            currentErrorColor = IWColors::IwRed;
        }
    }
    resetSleepDelay();
    slint_loop();
}

void exitErrorScreen()
{
    debugLog("Exiting error screen: " + errorScreenStr);
    errorScreenStr = "";
    setRgb(IWColors::IwNone);
    slintExit();
}