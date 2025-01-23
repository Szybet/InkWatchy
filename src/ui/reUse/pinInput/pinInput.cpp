#include "pinInput.h"

long long int *pinInputVar = NULL;
String newPin;
int currentPinButton;
#define BUTTON_BORDER_OFFSET 40
#define TOLERANCE 5
#define BORDER_WIDTH 1

#define OTHER_Y_CORD 120
#define ZERO_CORD 165, OTHER_Y_CORD
#define REMOVE_CORD -5, OTHER_Y_CORD

void showPin()
{
    dis->setTextWrap(false);
    dis->fillScreen(GxEPD_WHITE);
    setTextSize(1);
    uint16_t currentHeight = 20;
    // debugLog("Current height before center text: " + String(currentHeight));
    centerText(newPin, &currentHeight);
    // debugLog("Current heigh after center text: " + String(currentHeight));
    setTextSize(2);

    currentHeight = currentHeight + 5;
    int currentWidth = BUTTON_BORDER_OFFSET;

    for (int i = 1; i < 10; i++)
    {
        bool invert = false;
        if (i == currentPinButton)
        {
            invert = true;
        }
        sizeInfo size = drawButton(currentWidth, currentHeight, String(i), &emptyImgPack, invert, TOLERANCE, 0);
        currentWidth = currentWidth + size.w + TOLERANCE;
        if (i == 3 || i == 6)
        {
            currentWidth = BUTTON_BORDER_OFFSET;
            currentHeight = currentHeight + size.h + TOLERANCE;
        }
    }

    {
        bool invert = false;
        if (currentPinButton == 0)
        {
            invert = true;
        }
        drawButton(ZERO_CORD, "0", &emptyImgPack, invert, TOLERANCE, 0);
    }
    {
        bool invert = false;
        if (currentPinButton == 10)
        {
            invert = true;
        }
        drawButton(REMOVE_CORD, "r", &emptyImgPack, invert, TOLERANCE, 0);
    }

    dUChange = true;
}

void exitPinInput()
{
    debugLog("New pin is: " + String(newPin.toInt()));
    *pinInputVar = newPin.toInt();
    pinInputVar = NULL;
    debugLog("Still alive!");
}

void initPinInput()
{
    if (pinInputVar == NULL)
    {
        debugLog("pinInputVar is NULL, that's bad");
    }
    newPin = "";
    currentPinButton = 5;
    setFont(getFont("JackInput17"));
    maxHeight = 0;
    showPin();
}

void loopPinInput()
{
    switch (useButton())
    {
    case Up:
    {
        currentPinButton = currentPinButton - 1;
        checkMaxMin(&currentPinButton, 10);
        showPin();
        break;
    }
    case Down:
    {
        currentPinButton = currentPinButton + 1;
        checkMaxMin(&currentPinButton, 10);
        showPin();
        break;
    }
    case Menu:
    {
        if (currentPinButton != 10)
        {
            newPin = newPin + String(currentPinButton);
        }
        else
        {
            if (newPin.length() != 0)
            {
                newPin.remove(newPin.length() - 1);
            }
        }
        showPin();
        break;
    }
    default:
    {
        break;
    }
    }
    disUp();
}
