#include "wifiDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
#define SEL_ON 0
#define SEL_OFF 1
// Copy from file to file...
#define SEL_MAX 1
int selUi = 0;

#define cursorXwifi 0
int StatusHeight;
int IPHeight;
int SSIDHeight;
int SignalStrengthHeight;
int TaskStatusHeight;
#define TextSize 1

uint16_t wifiStatusLength;
uint16_t ipAddressLength;
uint16_t ssidLength;
uint16_t signalLength;
uint16_t taskStatusLenght;
String wifiStatusLastStr;
String taskStatusLastStr;
cordInfo onButtonCord;
cordInfo offButtonCord;
int wifiSignalLast;

void initWifiDebugDisplay()
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);
    display.setCursor(cursorXwifi, 1);
    String menuName = "Debug Menu: Wifi";
    getTextBounds(menuName, NULL, NULL, NULL, &maxHeight);

    uint16_t currentHeight = maxHeight;
    debugLog("maxHeight for wifi debug menu: " + String(maxHeight));
    maxHeight = maxHeight + 1; // Add here for more space between lines
    display.setCursor(cursorXwifi, currentHeight - 3);
    display.print(menuName);

    display.fillRect(0, currentHeight, display.width(), 3, GxEPD_BLACK);
    currentHeight = currentHeight + maxHeight;
    centerText("MAC address:", &currentHeight);

    String macAddressStr = WiFi.macAddress();
    getTextBounds(macAddressStr, NULL, NULL, &wifiStatusLength, NULL);
    centerText(macAddressStr, &currentHeight);

    centerText("Wifi status: ", &currentHeight);

    wifiStatusLastStr = wifiStatus();
    getTextBounds(wifiStatusLastStr, NULL, NULL, &wifiStatusLength, NULL);
    // wifiStatusLength = w;

    centerText(wifiStatusLastStr, &currentHeight);
    StatusHeight = currentHeight - maxHeight;
    writeLine("IP: " + WiFi.localIP().toString(), cursorXwifi, &currentHeight);
    IPHeight = currentHeight - maxHeight;

    writeLine("SSID: " + WiFi.SSID(), cursorXwifi, &currentHeight);
    SSIDHeight = currentHeight - maxHeight;

    writeLine("Wifi signal: " + String(getSignalStrength()) + "%", cursorXwifi, &currentHeight);
    SignalStrengthHeight = currentHeight - maxHeight;

    taskStatusLastStr = BOOL_STR(isWifiTaskCheck());
    getTextBounds("Connecting: " + taskStatusLastStr, NULL, NULL, &taskStatusLenght, NULL);
    writeLine("Connecting: " + taskStatusLastStr, cursorXwifi, &currentHeight);
    TaskStatusHeight = currentHeight - maxHeight;

    currentHeight = currentHeight - maxHeight + 6;
    display.fillRect(0, currentHeight, display.width(), 1, GxEPD_BLACK);
    currentHeight = currentHeight + 8;

    onButtonCord.x = 3;
    onButtonCord.y = currentHeight;
    offButtonCord.x = 70;
    offButtonCord.y = currentHeight;
    drawButton(onButtonCord.x, onButtonCord.y, "ON", acceptImgPack, true);
    drawButton(offButtonCord.x, offButtonCord.y, "OFF", crossImgPack);

    disUp(true);
}

void drawSelUi()
{
    // Is there a better solution?
    bool offBut = false;
    bool onBut = false;

    switch (selUi)
    {
    case SEL_ON:
    {
        onBut = true;
        break;
    }
    case SEL_OFF:
    {
        offBut = true;
        break;
    }
    }
    drawButton(onButtonCord.x, onButtonCord.y, "ON", acceptImgPack, onBut);
    drawButton(offButtonCord.x, offButtonCord.y, "OFF", crossImgPack, offBut);
    dUChange = true;
}

void loopWifiDebugDisplay()
{
    String wifiStatusStr = wifiStatus();
    //debugLog("Status here is: " + wifiStatusStr);
    uint16_t w;
    if (wifiStatusStr != wifiStatusLastStr)
    {
        wifiStatusLastStr = wifiStatusStr;
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        getTextBounds(wifiStatusStr, NULL, NULL, &w, NULL);
        debugLog("w: " + String(w) + " wifiStatusLength: " + String(wifiStatusLength));
        while (w < wifiStatusLength)
        {
            wifiStatusStr = " " + wifiStatusStr + " ";
            getTextBounds(wifiStatusStr, NULL, NULL, &w, NULL);
        }
        wifiStatusLength = w;

        writeTextCenterReplaceBack(wifiStatusStr, StatusHeight);

        String ipAddressStr = "IP: " + WiFi.localIP().toString();
        uint16_t ipWidth;
        getTextBounds(ipAddressStr, NULL, NULL, &ipWidth, NULL);
        debugLog("w: " + String(ipWidth) + " ipAddressLength: " + String(ipAddressLength));
        while (ipWidth < ipAddressLength)
        {
            ipAddressStr = ipAddressStr + " ";
            getTextBounds(ipAddressStr, NULL, NULL, &ipWidth, NULL);
        }
        ipAddressLength = ipWidth;

        writeTextReplaceBack(ipAddressStr, cursorXwifi, IPHeight);

        String ssidStr = "SSID: " + WiFi.SSID();
        uint16_t ssidWidth;
        getTextBounds(ssidStr, NULL, NULL, &ssidWidth, NULL);
        debugLog("w: " + String(ssidWidth) + " ssidLength: " + String(ssidLength));
        while (ssidWidth < ssidLength)
        {
            ssidStr = ssidStr + " ";
            getTextBounds(ssidStr, NULL, NULL, &ssidWidth, NULL);
        }
        ssidLength = ssidWidth;

        writeTextReplaceBack(ssidStr, cursorXwifi, SSIDHeight);
        dUChange = true;
    }
    int wifiSignal = getSignalStrength();
    if (wifiSignal != wifiSignalLast)
    {
        wifiSignalLast = getSignalStrength();
        wifiSignalLast = wifiSignal;
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        String signalStr = "Wifi signal: " + String(wifiSignal) + "%";
        uint16_t signalWidth;
        getTextBounds(signalStr, NULL, NULL, &signalWidth, NULL);
        debugLog("w: " + String(signalWidth) + " signalLength: " + String(signalLength));
        while (signalWidth < signalLength)
        {
            signalStr = signalStr + " ";
            getTextBounds(signalStr, NULL, NULL, &signalWidth, NULL);
        }
        signalLength = signalWidth;

        writeTextReplaceBack(signalStr, cursorXwifi, SignalStrengthHeight);
        dUChange = true;
    }
    String wifiTaskStatus = BOOL_STR(isWifiTaskCheck());
    //debugLog(wifiTaskStatus);
    if (wifiTaskStatus != taskStatusLastStr)
    {
        taskStatusLastStr = wifiTaskStatus;
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        // Update the TaskStatus
        String TaskStatusStr = "Connecting: " + wifiTaskStatus; // Replace with actual function or variable
        uint16_t taskStatusWidth;
        getTextBounds(TaskStatusStr, NULL, NULL, &taskStatusWidth, NULL);
        debugLog("w: " + String(taskStatusWidth) + " signalLength: " + String(taskStatusLenght));
        while (taskStatusWidth < taskStatusLenght)
        {
            TaskStatusStr = TaskStatusStr + " ";
            getTextBounds(TaskStatusStr, NULL, NULL, &taskStatusWidth, NULL);
        }
        taskStatusLenght = taskStatusWidth;

        writeTextReplaceBack(TaskStatusStr, cursorXwifi, TaskStatusHeight);
        dUChange = true;
    }

    //debugLog("selected: " + String(selUi));
    switch (useButton())
    {
    case Up:
    {
        selUi += 1;
        checkMaxMin(&selUi, SEL_MAX);
        drawSelUi();
        break;
    }
    case Down:
    {
        selUi -= 1;
        checkMaxMin(&selUi, SEL_MAX);
        drawSelUi();
        break;
    }
    case Menu:
    {
        switch (selUi)
        {
        case SEL_ON:
        {
            turnOnWifiRegular();
            vibrateMotor(VIBRATION_ACTION_TIME, true);
            break;
        }
        case SEL_OFF:
        {
            turnOffWifi();
            vibrateMotor(VIBRATION_ACTION_TIME, true);
            break;
        }
        }
        break;
    }
    }
    disUp();
}
#endif
