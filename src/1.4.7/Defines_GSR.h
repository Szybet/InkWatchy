#ifndef DEFINES_GSR_H
#define DEFINES_GSR_H

// WiFi
#define WiFi_AP_SSID "Watchy Connect"
#define WiFi_AP_PSWD "Watchy123"
#define WiFi_AP_HIDE false  // Hide "WiFi_AP_SSID" so it doesn't show up in the normal scan list.  IS NOT A SECURITY METHOD!
#define WiFi_AP_MAXC 4      // Maximum user connections (default is 4).
#define WiFi_SSID_MAX 32    // Do not change these two values.
#define WiFi_PASS_MAX 63
// Use instead of Watchy Connect (if necessary)
#define WiFi_DEF_SSID ""
#define WiFi_DEF_PASS ""

// Battery
#define GSR_MaxBattery 4.37

// functions
#define roller(v,lo,hi) (((v)<(lo))?(hi):((v)>(hi))?(lo):(v))
#define gobig(v,lo) ((v)>(lo)?(v):(lo))
#define golow(v,hi) ((v)<(hi)?(v):(hi))
#define isBitOn(var,pos) ((var) & (1<<(pos)))
#define setBitOn(var,pos) ((var) || (1<<(pos)))
#define BitValue(pos) (1<<(pos))

// Watch face states.
#define GSR_WATCHON 0
#define GSR_MENUON 1

// Maximum styles possible.
#define GSR_MaxStyles 96

// Nenu size defines.
#define GSR_MenuWidth 200
#define GSR_MenuHeight 83

// Color to tell system to use Auto foreground.
#define GSR_AutoFore 0xC0C0

// Menu offsets so I don't have to statically entere them everywhere.
#define GSR_MENU_STEPS 0
#define GSR_MENU_ALARMS 1
#define GSR_MENU_TIMERS 2
#define GSR_MENU_OPTIONS 3
#define GSR_MENU_ALARM1 4
#define GSR_MENU_ALARM2 5
#define GSR_MENU_ALARM3 6
#define GSR_MENU_ALARM4 7
#define GSR_MENU_TONES 8
#define GSR_MENU_TIMEDN 9
#define GSR_MENU_TMEDCF 10
#define GSR_MENU_TIMEUP 11
#define GSR_MENU_STYL 12
#define GSR_MENU_LANG 13
#define GSR_MENU_DISP 14
#define GSR_MENU_BRDR 15
#define GSR_MENU_SIDE 16
#define GSR_MENU_SWAP 17
#define GSR_MENU_ORNT 18
#define GSR_MENU_MODE 19
#define GSR_MENU_FEED 20
#define GSR_MENU_TRBO 21
#define GSR_MENU_DARK 22
#define GSR_MENU_SAVE 23
#define GSR_MENU_TPWR 24
#define GSR_MENU_INFO 25
#define GSR_MENU_TRBL 26
#define GSR_MENU_SYNC 27
#define GSR_MENU_WEAT 28
#define GSR_MENU_WIFI 29
#define GSR_MENU_OTAU 30
#define GSR_MENU_OTAM 31
#define GSR_MENU_SCRN 32
#define GSR_MENU_RSET 33
#define GSR_MENU_TOFF 34  // Time Diff offset.
#define GSR_MENU_UNVS 35

// Menu segments.
#define GSR_MENU_INNORMAL 0
#define GSR_MENU_INALARMS 1
#define GSR_MENU_INTIMERS 2
#define GSR_MENU_INOPTIONS 3
#define GSR_MENU_INTROUBLE 4

//ALARM flags.
#define GSR_ALARM_REPEAT 128
#define GSR_ALARM_ACTIVE 256
#define GSR_ALARM_NOTRIGGER 511
#define GSR_ALARM_TRIGGERED 512
#define GSR_ALARM_DAYS 127
#define GSR_ALARM_ALL 1023

// Setup lengths.
#define GSR_AlarmSetup 1234567890

//pins
#define GSR_RTC_INT_PIN 27
#define EPD_CS 5
#define EPD_DC 10
#define EPD_RESET 9
#define EPD_BUSY 19
#define GSR_VIB_MOTOR_PIN 13
#define GSR_MENU_PIN 26
#define GSR_BACK_PIN 25
#define GSR_DOWN_PIN 4
#define GSR_MENU_MASK GPIO_SEL_26
#define GSR_BACK_MASK GPIO_SEL_25
#define GSR_DOWN_MASK GPIO_SEL_4

//SetCPU defines.
#define GSR_CPUMAX 65280
#define GSR_CPUDEF 65281
#define GSR_CPUMID 65282
#define GSR_CPULOW 65283
#endif
