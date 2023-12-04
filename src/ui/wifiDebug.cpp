#include "wifiDebug.h"

#if DEBUG == 1 || DEBUG_MENUS == 1
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
int wifiSignalLast;

void initWifiDebugDisplay()
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(TextSize);
    display.setCursor(cursorXwifi, 1);
    String menuName = "Debug Menu: Wifi";
    getTextBounds(menuName, NULL, NULL, NULL, &maxHeight);

    uint16_t currentHeight = maxHeight;
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

    taskStatusLastStr = BOOL_STR(isWifiTaskRunning);
    getTextBounds("Connecting: " + taskStatusLastStr, NULL, NULL, &taskStatusLenght, NULL);
    writeLine("Connecting: " + taskStatusLastStr, cursorXwifi, &currentHeight);
    TaskStatusHeight = currentHeight - maxHeight;

    display.fillRect(0, currentHeight - maxHeight / 2 - 2, display.width(), 1, GxEPD_BLACK);
    currentHeight = currentHeight + 5;

    writeLine("Turn wifi on", cursorXwifi, &currentHeight);
    writeLine("Turn wifi off", cursorXwifi, &currentHeight);
    display.display(FULL_UPDATE);
}

void loopWifiDebugDisplay()
{
    String wifiStatusStr = wifiStatus();
    log("Status here is: " + wifiStatusStr);
    uint16_t w;
    if (wifiStatusStr != wifiStatusLastStr)
    {
        wifiStatusLastStr = wifiStatusStr;
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        getTextBounds(wifiStatusStr, NULL, NULL, &w, NULL);
        log("w: " + String(w) + " wifiStatusLength: " + String(wifiStatusLength));
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
        log("w: " + String(ipWidth) + " ipAddressLength: " + String(ipAddressLength));
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
        log("w: " + String(ssidWidth) + " ssidLength: " + String(ssidLength));
        while (ssidWidth < ssidLength)
        {
            ssidStr = ssidStr + " ";
            getTextBounds(ssidStr, NULL, NULL, &ssidWidth, NULL);
        }
        ssidLength = ssidWidth;

        writeTextReplaceBack(ssidStr, cursorXwifi, SSIDHeight);
        display.display(PARTIAL_UPDATE);
        int guwno;
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
        log("w: " + String(signalWidth) + " signalLength: " + String(signalLength));
        while (signalWidth < signalLength)
        {
            signalStr = signalStr + " ";
            getTextBounds(signalStr, NULL, NULL, &signalWidth, NULL);
        }
        signalLength = signalWidth;

        writeTextReplaceBack(signalStr, cursorXwifi, SignalStrengthHeight);
        display.display(PARTIAL_UPDATE);
    }
    String wifiTaskStatus = BOOL_STR(isWifiTaskRunning);
    log(wifiTaskStatus);
    if (wifiTaskStatus != taskStatusLastStr)
    {
        taskStatusLastStr = wifiTaskStatus;
        setFont(&FreeSansBold9pt7b);
        setTextSize(TextSize);

        // Update the TaskStatus
        String TaskStatusStr = "Connecting: " + wifiTaskStatus; // Replace with actual function or variable
        uint16_t taskStatusWidth;
        getTextBounds(TaskStatusStr, NULL, NULL, &taskStatusWidth, NULL);
        log("w: " + String(taskStatusWidth) + " signalLength: " + String(taskStatusLenght));
        while (taskStatusWidth < taskStatusLenght)
        {
            TaskStatusStr = TaskStatusStr + " ";
            getTextBounds(TaskStatusStr, NULL, NULL, &taskStatusWidth, NULL);
        }
        taskStatusLenght = taskStatusWidth;

        writeTextReplaceBack(TaskStatusStr, cursorXwifi, TaskStatusHeight);
        display.display(PARTIAL_UPDATE);
    }
    /*
        buttonState clicked = useButton();
        if(clicked != None) {
            if(clicked == UP) {

            }
        }
        */
}
#endif
