#include "apiMod.h"

#if API_MODULE

void wfApicheckShow(bool *showBool, bool *redrawBool)
{
    *showBool = true;
}

void wfApirequestShow(buttonState button, bool *showBool)
{
    switch (button)
    {
    case Menu:
    {
        if (watchfacePos == MODULE_ENG_POS && positionEngaged == true)
        {
            if (disableSomeDrawing == false)
            {
                disableSomeDrawing = true;
            }
            else
            {
                disableSomeDrawing = false;
                showFullWatchface();
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if (disableSomeDrawing == false)
    {
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        display.setCursor(MODULE_RECT_X, MODULE_RECT_Y + MODULE_H / 2);
        display.print("Api awaits");
    }
    else
    {
        // true

        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        maxHeight = 17;
        uint16_t currLine = SOME_RECT_Y + 10;

        cleanSomeDrawing();
        writeLine("Api active", MODULE_RECT_X, &currLine);
        if(button > 1 && button != Menu) {
            writeLine("Received button: " + String(button), MODULE_RECT_X, &currLine);
            bool conn1 = connectWifiQuick();
            writeLine("Wifi quick: " + BOOL_STR(conn1), MODULE_RECT_X, &currLine);
            if(conn1 == true) {
                WiFiClient client;
                IPAddress ip(WQ_CONN_TO);
                bool conn2 = client.connect(ip, WQ_PORT_TO);
                writeLine("Connected: " + BOOL_STR(conn2), MODULE_RECT_X, &currLine);
                if (conn2 == true) {
                    client.setTimeout(WIFI_QUICK_MAX_MS / 2);
                    size_t wrote = client.print(String(button));
                    client.stop();
                    writeLine("Written: " + String(wrote), MODULE_RECT_X, &currLine);
                }
                writeLine("The end", MODULE_RECT_X, &currLine);
                disableWatchfaceFastOperating = true;
                disconnectWifiQuick();
            }
        }
    }
    dUChange = true;
}

// Lambda doesn't work here
RTC_DATA_ATTR wfModule wfApi = {
    true,
    wfApicheckShow,
    wfApirequestShow,
};

#endif