#include "apiMod.h"
#include "rtcMem.h"

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
        if (isModuleEngaged() == true)
        {
            if (rM.disableSomeDrawing == false)
            {
                rM.disableSomeDrawing = true;
            }
            else
            {
                rM.disableSomeDrawing = false;
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

    squareInfo modSq = getWatchModuleSquare();
    squareInfo someSq = getSomeDrawingSquare();
    if (rM.disableSomeDrawing == false)
    {
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        dis->setCursor(modSq.cord.x, modSq.cord.y + modSq.size.h / 2);
        dis->print("Api awaits");
    }
    else
    {
        // true
        setFont(&FreeSansBold9pt7b);
        setTextSize(1);
        maxHeight = 17;
        uint16_t currLine = someSq.cord.y + 10;

        cleanSomeDrawing();
        writeLine("Api active", modSq.cord.x, &currLine);
        if(button > 1 && button != Menu) {
            writeLine("Received button: " + String(button), modSq.cord.x, &currLine);
            bool conn1 = connectWifiQuick();
            writeLine("Wifi quick: " + BOOL_STR(conn1), modSq.cord.x, &currLine);
            if(conn1 == true) {
                WiFiClient client;
                IPAddress ip(WQ_CONN_TO);
                bool conn2 = client.connect(ip, WQ_PORT_TO);
                writeLine("Connected: " + BOOL_STR(conn2), modSq.cord.x, &currLine);
                if (conn2 == true) {
                    client.setTimeout(WIFI_QUICK_MAX_MS / 2);
                    size_t wrote = client.print(String(button));
                    client.stop();
                    writeLine("Written: " + String(wrote), modSq.cord.x, &currLine);
                }
                writeLine("The end", modSq.cord.x, &currLine);
                disableWatchfaceFastOperating = true;
                disconnectWifiQuick();
            }
        }
    }
    dUChange = true;
}

#endif