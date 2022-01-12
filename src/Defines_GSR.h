#ifndef DEFINES_GSR_H
#define DEFINES_GSR_H

// Time Sync Server
#define ntpServer "pool.ntp.org"

//debug
#define USEDEBUG 0  // !0 is on, will not setup Serial OR print output if zero.

// WiFi
#define WiFi_AP_SSID "Watchy Connect"
#define WiFi_AP_PSWD "Watchy123"
#define WiFi_AP_HIDE false
#define WiFi_AP_MAXC 4
#define WiFi_SSID_MAX 32
#define WiFi_PASS_MAX 63
// Use instead of Watchy Connect (if necessary)
#define WiFi_DEF_SSID ""
#define WiFi_DEF_PASS ""

// Battery
#define MaxBattery 4.37
#define MinBattery 3.58
#define LowBattery 3.45

// functions
#define roller(v,lo,hi) (((v)<(lo))?(hi):((v)>(hi))?(lo):(v))
#define gobig(v,lo) ((v)>(lo)?(v):(lo))
#define golow(v,hi) ((v)<(hi)?(v):(hi))

// Watch face states.
#define WATCHON 0
#define MENUON 1

// Nenu size defines.
#define MenuWidth 200
#define MenuHeight 83

// BMA For Tilt/DTap
#define BMA432_INT1_PIN 14
#define BMA432_INT2_PIN 12
#define BMA432_INT1_MASK (1<<BMA432_INT1_PIN)
#define BMA432_INT2_MASK (1<<BMA432_INT2_PIN)

// Menu offsets so I don't have to statically entere them everywhere.
#define MENU_STEPS 0
#define MENU_ALARMS 1
#define MENU_TIMERS 2
#define MENU_OPTIONS 3
#define MENU_ALARM1 4
#define MENU_ALARM2 5
#define MENU_ALARM3 6
#define MENU_ALARM4 7
#define MENU_TONES 8
#define MENU_TIMEDN 9
#define MENU_TIMEUP 10
#define MENU_DISP 11
#define MENU_BRDR 12
#define MENU_SIDE 13
#define MENU_SWAP 14
#define MENU_ORNT 15
#define MENU_MODE 16
#define MENU_FEED 17
#define MENU_TRBO 18
#define MENU_DARK 19
#define MENU_SAVE 20
#define MENU_TPWR 21
#define MENU_INFO 22
#define MENU_TRBL 23
#define MENU_SYNC 24
#define MENU_WIFI 25
#define MENU_OTAU 26
#define MENU_OTAM 27
#define MENU_SCRN 28
#define MENU_RSET 29
#define MENU_TOFF 30  // Time Diff offset.
#define MENU_UNVS 31

// Menu segments.
#define MENU_INNORMAL 0
#define MENU_INALARMS 1
#define MENU_INTIMERS 2
#define MENU_INOPTIONS 3
#define MENU_INTROUBLE 4

// Button debounce.
#define KEYPAUSE 728

//ALARM flags.
#define ALARM_REPEAT 128
#define ALARM_ACTIVE 256
#define ALARM_NOTRIGGER 511
#define ALARM_TRIGGERED 512
#define ALARM_DAYS 127
#define ALARM_ALL 1023

// Setup lengths.
#define AlarmSetup 1234567890

//pins
#define RTC_PIN GPIO_NUM_27
#define CS 5
#define DC 10
#define RESET 9
//#define BUSY 19
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define UP_BTN_PIN 32
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define UP_BTN_MASK GPIO_SEL_32
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

//SetCPU defines.
#define CPUMAX 65280
#define CPUDEF 65281
#define CPUMID 65282
#endif
