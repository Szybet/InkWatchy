#pragma once

#include "minimalDefines.h"

struct ImageDef
{
    int16_t bw;
    int16_t bh;
    uint8_t *bitmap;
};

extern ImageDef emptyImgPack;

// It's converted to int by api mod
typedef enum
{
    Unknown = 0, // For the task to look for answers, used on the yatchy
    None = 1,
    Back = 2,
    Menu = 3,
    Up = 4,
    Down = 5,
    LongBack = 6,
    LongMenu = 7,
    LongUp = 8,
    LongDown = 9,
} buttonState; // This needs to be here because watchface modules use it too and idk why it doesn't work if its in buttons.h :( send help

typedef enum
{
    WifiOff = 0,
    WifiOn = 1,
    WifiConnected = 2,
    WifiUndefined = 3,
} wifiStatusSimple; // This too

struct wfModule
{
    bool show;
    void (*checkShow)(bool *showBool, bool *redrawBool);
    void (*requestShow)(buttonState button, bool *showBool);
}; // Madness -,-

struct bufSize
{
    uint8_t *buf;
    int size;
};

extern bufSize emptyBuff;

#define genPointer uintptr_t // Generic pointer

typedef struct
{
    const char *ssid;
    const char *password;
} WiFiCred;

#define STATIC_WIFI_CRED static const WiFiCred

struct sizeInfo
{
    uint16_t w;
    uint16_t h;
};

struct cordInfo
{
    uint16_t x;
    uint16_t y;
};

struct squareInfo
{
    sizeInfo size;
    cordInfo cord;
};

enum watchFaceManager
{
    wfmNone = -1, // None, doesn't exist
    wfmOne = 1,   // InkWatchy default, Szybet certified
    wfmTwo = 2,   // Do what you want mode
    wfmGSR = 3,   // GSR compability layer
};

enum wfmTwoArg
{
    wTloop,
    wTinit,
};

enum wfmTwoRet
{
    wTNone,
    wTYes,
    wTNo,
};

struct watchfaceDef
{
    watchFaceManager manager;
    const char *name;
    genPointer data;
};

struct wifiQuickCred
{
    const char *ssid;
    const char *pass;
    const char *bssid;
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns1;
    IPAddress dns2;
    IPAddress connectTo;
    uint port;
};