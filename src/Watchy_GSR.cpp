#include "Watchy_GSR.h"

// Debug message code.
bool SerialSetup;

//Timezone configuration
static const char TZURL[] PROGMEM = "http://ip-api.com/json";
static const char UserAgent[] PROGMEM = "Watchy";

//  AlarmVBs[] = {"0111111110", "0011001100", "0110110110", "0101001010"};
int AlarmVBs[] = {0x01FE, 0x00CC, 0x01B6, 0x014A};
const uint16_t Bits[10] = {1,2,4,8,16,32,64,128,256,512};
const float Reduce[5] = {1.0,0.8,0.6,0.4,0.2};

RTC_DATA_ATTR struct GSRWireless {
    bool Requested;          // Request WiFi.
    bool Working;            // Working on getting WiFi.
    bool Results;            // Results of WiFi, found an AP?
    uint8_t Index;           // 10 = built-in, roll backwards to 0.
    struct APInfo {
        char APID[33];
        char PASS[64];
    } AP[10];                // Using APID to avoid internal confusion with SSID.
    unsigned long Last;      // Used with millis() to maintain sanity.
} GSRWiFi;

RTC_DATA_ATTR struct Stepping {
    uint8_t Hour;
    uint8_t Minutes;
    bool Reset;
    uint32_t Yesterday;
} Steps;

RTC_DATA_ATTR struct Optional {
    bool TwentyFour;                  // If the face shows 24 hour or Am/Pm.
    bool LightMode;                    // Light/Dark mode.
    bool Feedback;                    // Haptic Feedback on buttons.
    bool Border;                      // True to set the border to black/white.
    bool Lefty;                       // Swaps the buttons to the other side.
    bool Swapped;                     // Menu and Back buttons swap ends (vertically).
    bool Orientated;                  // Set to false to not bother which way the buttons are.
    uint8_t Turbo;                    // 0-10 seconds.
    uint8_t MasterRepeats;            // Done for ease, will be in the Alarms menu.
} Options;

RTC_DATA_ATTR int GuiMode;
RTC_DATA_ATTR bool ScreenOn;              // Screen needs to be on.
RTC_DATA_ATTR bool VibeMode;              // Vibe Motor is On=True/Off=False, used for the Haptic and Alarms.
RTC_DATA_ATTR String WatchyStatus;        // Used for the indicator in the bottom left, so when it changes, it asks for a screen refresh, if not, it doesn't.

RTC_DATA_ATTR struct TimeData {
    time_t UTC_RAW;           // Copy of the UTC on init.
    tmElements_t UTC;         // Copy of UTC only split up for usage.
    tmElements_t Local;       // Copy of the Local time on init.
    String TimeZone;          // The location timezone, not the actual POSIX.
    char POSIX[64];           // The POSIX result.
    String LastTime;          // Used for determining if the time display changed.
    String LastDay;
    String LastDate;
    String LastYear;
} WatchTime;

RTC_DATA_ATTR struct Countdown {
  bool Active;
  uint8_t Hours;
  uint8_t Mins;
  uint8_t MaxHours;
  uint8_t MaxMins;
  uint8_t Tone;       // 10 to start.
  uint8_t ToneLeft;   // 30 to start.
  uint8_t MaxTones;   // 0-4 (20-80%) tone duration reduction.
  time_t LastUTC;
} TimerDown;

RTC_DATA_ATTR struct CountUp {
  bool Active;
  time_t SetAt;
  time_t StopAt;
} TimerUp;

RTC_DATA_ATTR struct BatteryUse {
    float Last;             // Used to track battery changes, only updates past 0.01 in change.
    int8_t Direction;       // -1 for draining, 1 for charging.
    int8_t UpCount;         // Counts how many times the battery is in a direction to determine true charging.
    int8_t DownCount;
    int8_t LastState;       // 0=not visible, 1= showing chargeme, 2=showing charging.
} Battery;

RTC_DATA_ATTR struct MenuUse {
    int8_t Style;           // MENU_INNORMAL or MENU_INOPTIONS
    int8_t Item;            // What Menu Item is being viewed.
    int8_t SubItem;         // Used for menus that have sub items, like alarms and Sync Time.
    int8_t SubSubItem;      // Used mostly in the alarm to offset choice.
    String LastHeader;      // Used for checking menu state needing update.
    String LastItem;
} Menu;

RTC_DATA_ATTR struct NTPUse {
    uint8_t State;          // State = 0=Off, 1=Start WiFi, 2=Wait for WiFi, TZ, send NTP request, etc, Finish.  See function ProcessNTP();
    uint8_t Wait;           // Counts up to 3 minutes, then fails.
    uint8_t Pause;          // How many 50ms to pause for.
    time_t Last;            // Last time it worked.
    bool TimeZone;          // Update Timezone during ProcessNTP.
    bool UpdateUTC;         // Update UTC during ProcessNTP.
} NTPData;

struct dispUpdate {
    bool Time;
    bool Day;
    bool Date;
    bool Header;
    bool Item;
    bool Status;
    bool Year;
    bool Charge;
    bool Full;
} Updates;

struct SpeedUp {
    bool On;
    time_t Last;
} Turbo;

DS3232RTC WatchyGSR::RTC(false); 
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> WatchyGSR::display(GxEPD2_154_D67(CS, DC, RESET, BUSY));

volatile uint8_t Button;

//RTC_DATA_ATTR alignas(8) struct AlarmID {
RTC_DATA_ATTR     uint8_t   Alarms_Hour[4];
RTC_DATA_ATTR     uint8_t   Alarms_Minutes[4];
RTC_DATA_ATTR     uint16_t  Alarms_Active[4];         // Bits 0-6 (days of week), 7 Repeat, 8 Active, 9 Tripped (meaning if it isn't repeating, don't fire).
RTC_DATA_ATTR     uint8_t   Alarms_Times[4];          // Set to 10 to start, ends when zero.
RTC_DATA_ATTR     uint8_t   Alarms_Playing[4];        // Means the alarm tripped and it is to be played (goes to 0 when it finishes).
RTC_DATA_ATTR     uint8_t   Alarms_Repeats[4];        // 0-4 (20-80%) reduction in repetitions.
//} Alarms[4];

WiFiClient WiFiC;             // Tz
WiFiManager wifiManager;
HTTPClient HTTP;              // Tz
Olsen2POSIX OP;               // Tz code.
WebServer server(80);
unsigned long OTATimer;
bool WatchyAPOn;   // States Watchy's AP is on for connection.  Puts in Active Mode until back happens.
bool ActiveMode;   // Moved so it can be checked.
bool OTAUpdate;    // Internet based OTA Update.
bool OTAEnd;       // Means somewhere, it wants this to end, so end it.
int OTATry;        // Tries to connect to WiFi.
bool DoHaptic;    // Want it to happen after screen update.
bool UpdateDisp;   // Display needs to be updated.
unsigned long LastButton, OTAFail;

//    nvs_handle hNVS;

WatchyGSR::WatchyGSR(){}  //constructor

// Init Defaults after a reboot, setup all the variables here for defaults to avoid randomness.
void WatchyGSR::setupDefaults(){
    Options.TwentyFour = false;
    Options.LightMode = true;
    Options.Feedback = true;
    Options.Border = false;
    Options.Lefty = false;
    Options.Swapped = false;
    Options.Turbo = 3;
    Options.MasterRepeats = 0;  // 100%.
    Steps.Hour = 6;
    Steps.Minutes = 0;
}

void WatchyGSR::init(){
    uint64_t wakeupBit;
    time_t NTPWaiting, RightNOW, LastCheck;
    int AlarmIndex, Pushed;                          // Alarm being played.
    bool AlarmsOn, WaitForNext, Pulse, DoOnce;
    unsigned long Since, AlarmReset, APLoop;

    esp_sleep_wakeup_cause_t wakeup_reason;
    Wire.begin(SDA, SCL); //init i2c
    display.init(0, false); //_initial_refresh to false to prevent full update on init (moved here so it isn't done repeatedly during loops).

    pinMode(MENU_BTN_PIN, INPUT);   // Prep these for the loop below.
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);

    wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
    wakeupBit = esp_sleep_get_ext1_wakeup_status();
    DoOnce = true;

    switch (wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
            RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
            UpdateUTC();
            UpdateClock();
            detectBattery();
            UpdateDisp=true;
            break;
        case ESP_SLEEP_WAKEUP_EXT1: //button Press
            //if (wakeup_reason & ACC_INT_MASK) handleAccelerometer(); else 
            Pushed = getButtonMaskToID(wakeupBit);
            if (Pushed > 0) { Button = Pushed; LastButton = millis(); }
            break;
        default: //reset
            _rtcConfig();
            _bmaConfig();
            UpdateUTC();
            UpdateClock();
            setupDefaults();
            initZeros();
            AlarmIndex=0;
            AlarmsOn=false;
            WaitForNext=false;
            Updates.Full=true;
            UpdateDisp=true;
            //Init interrupts.
            attachInterrupt(digitalPinToInterrupt(MENU_BTN_PIN), std::bind(&WatchyGSR::handleInterrupt,this), HIGH);
            attachInterrupt(digitalPinToInterrupt(BACK_BTN_PIN), std::bind(&WatchyGSR::handleInterrupt,this), HIGH);
            attachInterrupt(digitalPinToInterrupt(UP_BTN_PIN), std::bind(&WatchyGSR::handleInterrupt,this), HIGH);
            attachInterrupt(digitalPinToInterrupt(DOWN_BTN_PIN), std::bind(&WatchyGSR::handleInterrupt,this), HIGH);
            break;
    }
    // Sometimes BMA crashes - simply try to reinitialize bma...
    if (sensor.getErrorCode() != 0) {
        sensor.shutDown();
        sensor.wakeUp();
        sensor.softReset();
        _bmaConfig();
    }

    if (Button > 0) { handleButtonPress(Button); Button = 0; }

    CalculateTones(); monitorSteps();
    AlarmsOn =(Alarms_Times[0] > 0 || Alarms_Times[1] > 0 || Alarms_Times[2] > 0 || Alarms_Times[3] > 0 || TimerDown.ToneLeft > 0);
    ActiveMode = (InTurbo() || NTPData.State > 0 || AlarmsOn || WatchyAPOn || OTAUpdate);

    while(DoOnce || Button > 0){
        DoOnce = false;  // Do this whole thing once, catch late button presses at the END of the loop just before Deep Sleep.
        if (UpdateDisp) showWatchFace(); //partial updates on tick
        NTPWaiting = WatchTime.UTC_RAW;
        RightNOW = WatchTime.UTC_RAW;
        Since=millis();
        LastCheck = RightNOW;
        AlarmReset = Since;
        OTATimer = Since;
        OTAFail = Since;
        if (ActiveMode){
            while (ActiveMode == true) { // Here, we hijack the init and LOOP until the NTP is done, watching for the proper time when we *SHOULD* update the screen to keep time with everything.

              processWiFiRequest(); // Process any WiFi requests.

                if (!(OTAUpdate && Menu.SubItem == 3)){
                    if (NTPData.State > 0 && !WatchyAPOn && !OTAUpdate){
                        if (NTPData.Pause == 0) ProcessNTP(); else NTPData.Pause--;
                        if (RightNOW - NTPWaiting >= 60){
                            NTPWaiting = WatchTime.UTC_RAW;
                            NTPData.Wait++;
                            UpdateDisp=true;
                        }
                    }

                    // Here, make sure the clock updates on-screen.
                    if (WatchTime.UTC_RAW - LastCheck >= 60){
                        LastCheck = WatchTime.UTC_RAW;
                        detectBattery();
                        UpdateDisp = true;
                    }

                    // Here, do the alarm buzzings by way of which one is running.

                    if (AlarmsOn && !OTAUpdate){
                        if (WaitForNext){
                            // Wait for the next second to change between any alarms still to play.
                            if (millis() - AlarmReset > 999){ // Time changed.
                                AlarmReset = millis();
                                WaitForNext = false;
                                AlarmIndex = roller(AlarmIndex + 1, 0, 4);
                                if (getToneTimes(AlarmIndex) == 0){
                                    AlarmIndex = roller(AlarmIndex + 1, 0, 4);
                                    if (getToneTimes(AlarmIndex) == 0){
                                        AlarmIndex = roller(AlarmIndex + 1, 0, 4);
                                        if (getToneTimes(AlarmIndex) == 0){
                                            AlarmIndex = roller(AlarmIndex + 1, 0, 4);
                                            if (getToneTimes(AlarmIndex) == 0){
                                                AlarmIndex = roller(AlarmIndex + 1, 0, 4);
                                            }
                                        }
                                    }
                                }
                            }
                        }else if(AlarmIndex == 4){
                            if (TimerDown.ToneLeft > 0){
                                if (TimerDown.Tone == 0){
                                    WaitForNext=true;
                                    if (TimerDown.ToneLeft > 0){
                                        TimerDown.ToneLeft--;
                                        TimerDown.Tone = 24;
                                    }
                                }else{
                                    TimerDown.Tone--;
                                    Pulse = (((TimerDown.Tone / 2) & 1) != 0);
                                    if (!Pulse && TimerDown.Tone > 0) TimerDown.Tone--;
                                    VibeTo(Pulse);   // Turns Vibe on or off depending on bit state.
                                }
                            }else WaitForNext=true;
                        }else if (Alarms_Times[AlarmIndex] > 0){
                            if (Alarms_Playing[AlarmIndex] > 0){
                                Alarms_Playing[AlarmIndex]--;
                                if (Menu.SubItem > 0 && Menu.Item - MENU_ALARM1 == AlarmIndex){
                                    VibeTo(false);
                                    DoHaptic = false;
                                    Alarms_Playing[AlarmIndex]=0;
                                    Alarms_Times[AlarmIndex]=0;
                                }else{
                                    Pulse = ((AlarmVBs[AlarmIndex] & Bits[Alarms_Playing[AlarmIndex] / 3]) != 0);
                                    VibeTo(Pulse);   // Turns Vibe on or off depending on bit state.
                                    DoHaptic = false;
                                }
                                if (Alarms_Playing[AlarmIndex] == 0 && Alarms_Times[AlarmIndex] > 0){
                                    Alarms_Times[AlarmIndex]--;   // Decrease count, eventually this will all stop on it's own.
                                    WaitForNext = true;
                                    if (Alarms_Times[AlarmIndex] > 0) Alarms_Playing[AlarmIndex] = 30;
                                }
                            }else WaitForNext = true;
                        }else WaitForNext = true;
                    }

                    if (WatchyAPOn && !OTAUpdate){
                        switch (Menu.SubItem){
                            case 0: // Turn off AP.
                                OTAEnd = true;
                                break;
                            case 1: // Turn on AP.
                                if (WiFi.getMode() != WIFI_AP || (millis() - OTATimer > 4000 && OTATry < 3)){
                                    OTATimer=millis();
                                    OTATry++;
                                    WiFi.setHostname(WiFi_AP_SSID);
                                    wifiManager.setConfigPortalBlocking(false);
                                    wifiManager.setWiFiAPHidden(WiFi_AP_HIDE);
                                    if (!WiFi.softAP(WiFi_AP_SSID,WiFi_AP_PSWD,1,WiFi_AP_HIDE)) OTAEnd = true;
                                }else if (WiFi.getMode() == WIFI_AP){
                                    wifiManager.startWebPortal();
                                    Menu.SubItem++;
                                    setStatus("WiFi-AP");
                                    UpdateDisp=true;
                                    APLoop=millis();
                                }else Menu.SubItem = 0; // Fail, something is amiss.
                                break;
                            default: // 2 to 5 is here.
                                if (Menu.SubItem > 1){
                                    if(WiFi.getMode() == WIFI_STA){
                                        Menu.SubItem = 0;
                                        break;
                                    }
                                    if (wifiManager.process()){ // Setting worked.
                                        Menu.SubItem = 0;
                                        break;
                                    }
                                    if (millis() - APLoop > 8000){
                                        Menu.SubItem = roller(Menu.SubItem + 1, 2,4);
                                        UpdateDisp = true;
                                        APLoop=millis();
                                    }
                                }
                        }
                    }
                }
                if (OTAUpdate){
                  switch (Menu.SubItem){
                      case 1: // Wait for WiFi to connect or fail.
                          if (WiFi.status() != WL_CONNECTED && currentWiFi() != WL_CONNECT_FAILED) OTATimer = millis();
                          else if (WiFi.status() == WL_CONNECTED){
                              Menu.SubItem++;
                              UpdateDisp = true;
                          }else OTAEnd=true;
                          break;
                      case 2: // Setup Arduino OTA and wait for it to either finish or fail by way of back button held for too long OR 2 minute with no upload.
                          if (Menu.Item == MENU_OTAU){
                          ArduinoOTA.setHostname(WiFi_AP_SSID);
                          ArduinoOTA
                            .onStart([]() {
                              String Type;
                              if (ArduinoOTA.getCommand() == U_FLASH)
                                Type = "sketch";
                              else // U_SPIFFS
                                Type = "filesystem";
                              // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                              //Serial.println("Start updating " + Type);
                            })
                            .onEnd([]() {
                              OTAEnd = true;
                              //Serial.println("\nEnd");
                            })
                            .onProgress([](unsigned int progress, unsigned int total) {
                              OTATimer=millis();
                              //Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
                            })
                            .onError([](ota_error_t error) {
                              /*Serial.printf("Error[%u]: ", error);
                              if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                              else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                              else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                              else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                              else if (error == OTA_END_ERROR) Serial.println("End Failed");
                              */
                            });
                          ArduinoOTA.begin();
                          }else if (Menu.Item == MENU_OTAM){
                              /*return index page which is stored in basicIndex */
                              server.on("/", HTTP_GET, [=]() {
                                server.sendHeader("Connection", "close");
                                server.send(200, "text/html", basicIndex);
                                OTATimer=millis();
                              });
                              server.on("/settings", HTTP_GET, [=]() {
                                String S = settingsA + GetSettings() + settingsB;
                                server.sendHeader("Connection", "close");
                                server.send(200, "text/html", S);
                                OTATimer=millis();
                              });
                              server.on("/wifi", HTTP_GET, [=]() {
                                server.sendHeader("Connection", "close");
                                server.send(200, "text/html", buildWiFiAPPage());
                                OTATimer=millis();
                              });
                              server.on("/update", HTTP_GET, [=]() {
                                server.sendHeader("Connection", "close");
                                server.send(200, "text/html", updateIndex);
                                OTATimer=millis();
                              });
                              server.on("/settings", HTTP_POST, [=](){
                                  if (server.argName(0) == "settings") StoreSettings(server.arg(0));
                                  server.sendHeader("Connection", "close");
                                  server.send(200, "text/html", settingsDone);
                                  OTATimer=millis();
                              });
                              server.on("/wifi", HTTP_POST, [=](){
                                  uint8_t I = 0;
                                  while (I < server.args()){
                                      parseWiFiPageArg(server.argName(I),server.arg(I)); I++;
                                  }
                                  server.sendHeader("Connection", "close");
                                  server.send(200, "text/html", wifiDone);
                                  OTAFail = millis() - 598000;
                              });
                              server.on("/update", HTTP_POST, [](){
                                server.sendHeader("Connection", "close");
                                server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
                                ESP.restart();
                              }, []() {
                                HTTPUpload& upload = server.upload();
                                if (upload.status == UPLOAD_FILE_START) {
                                  OTATimer=millis();
                                  //Serial.printf("Update: %s\n", upload.filename.c_str());
                                  if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
                                    Update.printError(Serial);
                                  }
                                } else if (upload.status == UPLOAD_FILE_WRITE) {
                                  /* flashing firmware to ESP*/
                                  delay(2);
                                  if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) Update.printError(Serial);
                                  delay(2);
                                } else if (upload.status == UPLOAD_FILE_END) {
                                  if (Update.end(true)) { //true to set the size to the current progress
                                    OTAEnd=true;
                                  //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                                  } //else Update.printError(Serial);
                                }
                              });
                              server.begin();
                          }
                          Menu.SubItem++;
                          showWatchFace();
                          break;
                      case 3: // Monitor back button and turn WiFi off if it happens, or if duration is longer than 2 minutes.
                          if (Menu.Item == MENU_OTAU)      ArduinoOTA.handle();
                          else if (Menu.Item == MENU_OTAM) server.handleClient();
                          if (getButtonPins() != 2) OTATimer = millis(); // Not pressing "BACK".
                          if (millis() - OTATimer > 10000 || millis() - OTAFail > 600000) OTAEnd = true;  // Fail if holding back for 10 seconds OR 120 seconds has passed.
                    }
                }

                // OTAEnd code.
                if (OTAEnd){
                    //if (Menu.Item == MENU_OTAU)      ArduinoOTA.stop(); // Why is there no stop/close???
                    if (Menu.Item == MENU_OTAM) server.stop();
                    if (WatchyAPOn) wifiManager.stopConfigPortal();
                    VibeTo(false);
                    OTAEnd=false;
                    OTAUpdate=false;
                    WatchyAPOn = false;
                    endWiFi();
                    Menu.SubItem=0;
                    UpdateUTC();
                    UpdateClock();
                    Battery.UpCount=0;  // Stop it from thinking the battery went wild.
                    UpdateDisp=true;
                }

                // Don't do anything time sensitive while in OTA Update.
                if (!(OTAUpdate && Menu.SubItem == 3)){
                    CalculateTones(); monitorSteps();
                    AlarmsOn =(Alarms_Times[0] > 0 || Alarms_Times[1] > 0 || Alarms_Times[2] > 0 || Alarms_Times[3] > 0 || TimerDown.ToneLeft > 0);
                    ActiveMode = (InTurbo() || NTPData.State > 0 || AlarmsOn || WatchyAPOn || OTAUpdate);

                    // Here, check for button presses and respond, done here to avoid turbo button presses.

                    handleInterrupt();
                    if (Button > 0) { handleButtonPress(Button); Button = 0; }
                    if (UpdateDisp) showWatchFace(); //partial updates on tick

                    Since=50-(millis()-Since);
                    if (Since <= 50) delay(Since);
                    UpdateUTC();
                    RightNOW = WatchTime.UTC_RAW;
                    Since=millis();
                }
            }
        }

        if (Button > 0) { handleButtonPress(Button); Button = 0; }
        if (UpdateDisp) showWatchFace(); //partial updates on tick
    }
    deepSleep();
}

void WatchyGSR::showWatchFace(){
  display.epd2.setDarkBorder(Options.Border);
  drawWatchFace();
  if (Options.Feedback && DoHaptic){
    VibeTo(true);
    delay(40);
    VibeTo(false);
  }
  DoHaptic=false;
  UpdateDisp=false;
  ScreenRefresh();
}

void WatchyGSR::drawWatchFace(){
    uint8_t Direction;

    Direction = sensor.getDirection();  // Directional screen on.  == DIRECTION_DISP_UP

    display.fillScreen(Options.LightMode ? GxEPD_WHITE : GxEPD_BLACK);
    display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);

    ScreenOn = (Direction == DIRECTION_DISP_UP || Direction == DIRECTION_TOP_EDGE || Battery.Direction == 1 || true);  // True set due to bug.

    if (ScreenOn){
      if (!(OTAUpdate || WatchyAPOn)){
          drawTime();
          drawDay();
          drawYear();
      }
      if (GuiMode == WATCHON)     drawDate();
      else if (GuiMode == MENUON) drawMenu();
    }
    drawChargeMe();
    // Show WiFi/AP/TZ/NTP if in progress.
    drawStatus();
    UpdateDisp = false;
}

void WatchyGSR::drawTime(){
    int16_t  x1, y1;
    uint16_t w, h, tw;
    String O;
    bool PM;
    PM = false;
    O = MakeTime(WatchTime.Local.Hour, WatchTime.Local.Minute, PM);
    display.setFont(&aAntiCorona36pt7b);
    display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);

    display.getTextBounds(O, 0, TimeY, &x1, &y1, &w, &h);
    tw = (200 - w) /2;
    display.setCursor(tw, TimeY);
    display.println(O);

    if (PM){
        tw=clamp(tw + w + 6, 0, 184);
        display.drawBitmap(tw, TimeY - 45, PMIndicator, 6, 6, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
//        display.fillRect(tw, TimeY - 45 ,6 ,6, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
    }
    Updates.Time = (O != WatchTime.LastTime);
    WatchTime.LastTime = O;
}

void WatchyGSR::drawDay(){
    int16_t  x1, y1;
    uint16_t w, h;
    String O;

    O = dayStr(WatchTime.Local.Wday + 1);
    display.setFont(&aAntiCorona16pt7b);
    display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
    display.getTextBounds(O, 0, DayY, &x1, &y1, &w, &h);
    w = (200 - w) /2;
    display.setCursor(w, DayY);
    display.println(O);
    Updates.Day = (O != WatchTime.LastDay);
    WatchTime.LastDay = O;
}

void WatchyGSR::drawDate(){
    int16_t  x1, y1;
    uint16_t w, h;
    String O;

    display.setFont(&aAntiCorona15pt7b);  //Shahd_Serif17pt7b);
    display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
    O = String(monthStr(WatchTime.Local.Month)) + " " + String(WatchTime.Local.Day);
    //O="September 30";
    display.getTextBounds(O, 0, DateY, &x1, &y1, &w, &h);
    w = (200 - w) /2;
    display.setCursor(w, DateY);
    display.print(O);
    Updates.Date = (O != WatchTime.LastDate);
    WatchTime.LastDate = O;
}

void WatchyGSR::drawYear(){
    int16_t  x1, y1;
    uint16_t w, h, tw;
    String O;

    display.setFont(&aAntiCorona16pt7b);
    display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
    O = String(WatchTime.Local.Year + 1900);
    display.getTextBounds(O, 0, YearY, &x1, &y1, &w, &h);
    w = (200 - w) /2;
    display.setCursor(w, YearY);
    display.print(O);
    Updates.Year = (O != WatchTime.LastYear);
    WatchTime.LastYear = O;
}

void WatchyGSR::drawMenu(){
    int16_t  x1, y1;
    uint16_t w, h;
    String O, S;

    display.setFont(&aAntiCorona12pt7b);
    display.fillRect(0, MenuTop, 200, MenuHeight, Options.LightMode ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawBitmap(0, MenuTop, (Menu.Style == MENU_INOPTIONS) ? OptionsMenuBackground : MenuBackground, 200, MenuHeight, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(Options.LightMode && Menu.Style != MENU_INNORMAL ? GxEPD_WHITE : GxEPD_BLACK);
    switch (Menu.Item){
        case MENU_STEPS:
            if (Menu.SubItem > 0 && Menu.SubItem < 4) O = "Tomorrow";
            else if (Menu.SubItem == 4) O = "Reset Today";
            else O = "Steps";
            break;
        case MENU_ALARMS:
            O = "Alarms";
            break;
        case MENU_TIMERS:
            O = "Timers";
            break;
        case MENU_OPTIONS:
            O = "Options";
            break;
        case MENU_ALARM1:
            if (Menu.SubItem > 0 && Menu.SubItem < 4) O = "Alarm 1 Time";
            else if (Menu.SubItem == 4) O = "A1 Tone Repeats";
            else if (Menu.SubItem > 4) O = "Alarm 1 Options";
            else O = "Alarm 1";
            break;
        case MENU_ALARM2:
            if (Menu.SubItem > 0 && Menu.SubItem < 4) O = "Alarm 2 Time";
            else if (Menu.SubItem == 4) O = "A2 Tone Repeats";
            else if (Menu.SubItem > 4) O = "Alarm 2 Options";
            else O = "Alarm 2";
            break;
        case MENU_ALARM3:
            if (Menu.SubItem > 0 && Menu.SubItem < 4) O = "Alarm 3 Time";
            else if (Menu.SubItem == 4) O = "A3 Tone Repeats";
            else if (Menu.SubItem > 4) O = "Alarm 3 Options";
            else O = "Alarm 3";
            break;
        case MENU_ALARM4:
            if (Menu.SubItem > 0 && Menu.SubItem < 4) O = "Alarm 4 Time";
            else if (Menu.SubItem == 4) O = "A4 Tone Repeats";
            else if (Menu.SubItem > 4) O = "Alarm 4 Options";
            else O = "Alarm 4";
            break;
        case MENU_TONES:
            O = "Tone Repeats";
            break;
        case MENU_TIMEDN:
            O = "Countdown Timer";
            break;
        case MENU_TIMEUP:
            O = "Elapsed Timer";
            break;
        case MENU_DISP:
            O = "Display Style";
            break;
        case MENU_BRDR:
            O = "Border Mode";
            break;
        case MENU_SIDE:
            O = "Dexterity";
            break;
        case MENU_SWAP:
            O = "Menu & Back";
            break;
        case MENU_ORNT:
            O = "Orientation";
            break;
        case MENU_MODE:
            O = "Time Mode";
            break;
        case MENU_FEED:
            O = "Feedback";
            break;
        case MENU_TRBO:
            O = "Turbo Time";
            break;
        case MENU_SCRN:
            O = "Reset Screen";
            break;
        case MENU_SYNC:
            O = "Sync Watchy";
            break;
        case MENU_WIFI:
            O = "Watchy Connect";
            break;
        case MENU_OTAU:
            if (Menu.SubItem == 2 || Menu.SubItem == 3) O = "Upload Firmware"; else O = "OTA Update";
            break;
        case MENU_OTAM:
            if (Menu.SubItem == 2 || Menu.SubItem == 3) O = "Visit Website"; else O = "OTA Website";
            break;
        case MENU_RSET:
            O = "Watchy Reboot";
    }
    display.getTextBounds(O, 0, HeaderY, &x1, &y1, &w, &h);
    w = (196 - w) /2;
    display.setCursor(w + 2, HeaderY);
    display.print(O);
    Updates.Header = (O != Menu.LastHeader);
    Menu.LastHeader = O;
    display.setTextColor(GxEPD_BLACK);  // Only show menu in Light mode
    if (Menu.Item == MENU_STEPS){  //Steps
        switch (Menu.SubItem){
            case 0: // Steps.
              S = ""; if (Steps.Yesterday > 0) S = " (" + MakeSteps(Steps.Yesterday) + ")";
              O = MakeSteps(sensor.getCounter()) + S;
              break;
            case 1: // Hour.
              O="[" + MakeHour(Steps.Hour) + "]:" + MakeMinutes(Steps.Minutes) + (Steps.Hour > 11 ? " PM" : " AM") ;
              break;
            case 2: // x0 minutes.
              S=MakeMinutes(Steps.Minutes);
              O=MakeHour(Steps.Hour) + ":[" + S.charAt(0) + "]" + S.charAt(1) + (Steps.Hour > 11 ? " PM" : " AM") ;
              break;
            case 3: // 0x minutes.
              S=MakeMinutes(Steps.Minutes);
              O=MakeHour(Steps.Hour) + ":" + S.charAt(0) + "[" + S.charAt(1) + "]" + (Steps.Hour > 11 ? " PM" : " AM") ;
              break;
            case 4: // Sunday.
              O = "MENU to Reset";
          }
    }else if (Menu.Item == MENU_ALARMS){
            O = "MENU to Select";
    }else if (Menu.Item >= MENU_ALARM1 && Menu.Item <= MENU_ALARM4){  // Alarms
        O = "";
        S=MakeMinutes(Alarms_Minutes[Menu.Item - MENU_ALARM1]);
        switch (Menu.SubItem){
            case 0: // Menu to Edit.
              O="Menu to Edit";
              break;
            case 1: // Hour.
              O="[" + MakeHour(Alarms_Hour[Menu.Item - MENU_ALARM1]) + "]:" + S + ((Alarms_Hour[Menu.Item - MENU_ALARM1] > 11) ? " PM " : " AM ") + getReduce(Alarms_Repeats[Menu.Item - MENU_ALARM1]);
              break;
            case 2: // x0 minutes.
              O=MakeHour(Alarms_Hour[Menu.Item - MENU_ALARM1]) + ":[" + S.charAt(0) + "]" + S.charAt(1) + ((Alarms_Hour[Menu.Item - MENU_ALARM1] > 11) ? " PM " : " AM ") + getReduce(Alarms_Repeats[Menu.Item - MENU_ALARM1]);
              break;
            case 3: // 0x minutes.
              O=MakeHour(Alarms_Hour[Menu.Item - MENU_ALARM1]) + ":" + S.charAt(0) + "[" + S.charAt(1) + "]" + ((Alarms_Hour[Menu.Item - MENU_ALARM1] > 11) ? " PM " : " AM ") + getReduce(Alarms_Repeats[Menu.Item - MENU_ALARM1]);
              break;
            case 4: // Repeats
              O=MakeHour(Alarms_Hour[Menu.Item - MENU_ALARM1]) + ":" + S.charAt(0) + S.charAt(1) + ((Alarms_Hour[Menu.Item - MENU_ALARM1] > 11) ? " PM " : " AM ") + "[" + getReduce(Alarms_Repeats[Menu.Item - MENU_ALARM1]) + "]";
              break;
            case 5: // Sunday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[0]) == Bits[0]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(1);
              break;
            case 6: // Monday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[1]) == Bits[1]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(2);
              break;
            case 7: // Tuesday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[2]) == Bits[2]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(3);
              break;
            case 8: // Wedmesday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[3]) == Bits[3]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(4);
              break;
            case 9: // Thursday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[4]) == Bits[4]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(5);
              break;
            case 10: // Friday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[5]) == Bits[5]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(6);
              break;
            case 11: // Saturday.
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & Bits[6]) == Bits[6]) ? GxEPD_WHITE : GxEPD_BLACK);
              O=dayStr(7);
              break;
            case 12:  // Repeat
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & ALARM_REPEAT) == ALARM_REPEAT) ? GxEPD_WHITE : GxEPD_BLACK);
              O="Repeat";
              break;
            case 13:  // Active
              display.setTextColor(((Alarms_Active[Menu.Item - MENU_ALARM1] & ALARM_ACTIVE) == ALARM_ACTIVE) ? GxEPD_WHITE : GxEPD_BLACK);
              O="Active";
        }
    }else if (Menu.Item == MENU_TONES){   // Repeats on Alarms.
        O = getReduce(Options.MasterRepeats) + " repeats";
    }else if (Menu.Item == MENU_TIMERS){  // Timers
        O = "MENU to Select";
    }else if (Menu.Item == MENU_TIMEDN){ // Countdown
        S=MakeMinutes(TimerDown.Active ? TimerDown.Mins : TimerDown.MaxMins);
        switch (Menu.SubItem){
            case 0:
                O = "MENU to Edit";
                break;
            case 1: // Hours
                O="[" + String(TimerDown.Active ? TimerDown.Hours : TimerDown.MaxHours) + "]:" + S + " " + getReduce(TimerDown.MaxTones) + " " + (TimerDown.Active ? "Off" : "On");
                break;
            case 2: // 1x minutes.
                O=String(TimerDown.Active ? TimerDown.Hours : TimerDown.MaxHours) + ":[" + S.charAt(0) + "]" + S.charAt(1) + " " + getReduce(TimerDown.MaxTones) + " " + (TimerDown.Active ? "Off" : "On");
                break;
            case 3: // x1 minutes.
                O=String(TimerDown.Active ? TimerDown.Hours : TimerDown.MaxHours) + ":" + S.charAt(0) + "[" + S.charAt(1) + "] " + getReduce(TimerDown.MaxTones) + " " + (TimerDown.Active ? "Off" : "On");
                break;
            case 4: // %
                O=String(TimerDown.Active ? TimerDown.Hours : TimerDown.MaxHours) + ":" + S.charAt(0) + S.charAt(1) + " [" + getReduce(TimerDown.MaxTones) + "] " + (TimerDown.Active ? "Off" : "On");
                break;
            case 5: // Button.
                O=String(TimerDown.Active ? TimerDown.Hours : TimerDown.MaxHours) + ":" + S + " " + getReduce(TimerDown.MaxTones) + " " + (TimerDown.Active ? " [Off]" : " [On]");
        }
    }else if (Menu.Item == MENU_TIMEUP){ // Elapsed
        switch (Menu.SubItem){
            case 0:
                O = "MENU to Edit";
                break;
            case 1:
                if (TimerUp.Active) { y1 = ((WatchTime.UTC_RAW - (TimerUp.SetAt + WatchTime.UTC.Second)) / 3600); x1 = (((WatchTime.UTC_RAW - (TimerUp.SetAt + WatchTime.UTC.Second)) + 3600 * y1) / 60); }
                else { y1 = ((TimerUp.StopAt - TimerUp.SetAt) / 3600); x1 = (((TimerUp.StopAt - TimerUp.SetAt) + 3600 * y1) / 60); }
                O=String(y1) + ":" + MakeMinutes(x1) + (TimerUp.Active ? " [Off]" : " [On]");
        }
    }else if (Menu.Item == MENU_OPTIONS){ // Options Menu
        O = "MENU to Enter";
    }else if (Menu.Item == MENU_DISP){  // Switch Mode
        if (Options.LightMode){
            O = "Light";
        }else {
            O = "Dark";
        }
    }else if (Menu.Item == MENU_SIDE){  // Dexterity Mode
        if (Options.Lefty){
            O = "Left-handed";
        }else {
            O = "Right-handed";
        }
    }else if (Menu.Item == MENU_SWAP){  // Swap Menu/Back Buttons
        if (Options.Swapped){
            O = "Swapped";
        }else {
            O = "Normal";
        }
    }else if (Menu.Item == MENU_BRDR){  // Border Mode
        if (Options.Border){
            O = "Hide";
        }else {
            O = "Show";
        }
    }else if (Menu.Item == MENU_ORNT){  // Watchy Orientation.
        if (Options.Orientated){
            O = "Watchy UP";
        }else {
            O = "Ignore";
        }
    }else if (Menu.Item == MENU_MODE){  // 24hr Format Swap.
        if (Options.TwentyFour){
            O = "24 Hour";
        }else {
            O = "AM/PM";
        }
    }else if (Menu.Item == MENU_FEED){  // Feedback
        if (Options.Feedback){
            O = "Enabled";
        }else {
            O = "Disabled";
        }
    }else if (Menu.Item == MENU_TRBO){  // Turbo!
        if (Options.Turbo > 0) O=String(Options.Turbo) + " seconds"; else O = "Off";
    }else if (Menu.Item == MENU_SCRN){  // Reset Screen.
        O = "MENU to Reset";
    }else if (Menu.Item == MENU_SYNC){  // NTP
        if (Menu.SubItem == 0){
            O = "MENU to Start";
        }else if (Menu.SubItem == 1){
            O = "Time";
        }else if (Menu.SubItem == 2){
            O = "TimeZone";
        }else if (Menu.SubItem == 3){
            O = "TimeZone & Time";
        }
    }else if (Menu.Item == MENU_WIFI){ // Reset Steps.
        if (Menu.SubItem == 0){
            O = "MENU to Begin";
        }else if (Menu.SubItem == 1){
            O = "Starting AP";
        }else if (Menu.SubItem == 2){
            O = WiFi_AP_SSID;
        }else if (Menu.SubItem == 3){
            O = WiFi.softAPIP().toString();
        }else if (Menu.SubItem == 4){
            O = "BACK to End";
        }
    }else if (Menu.Item == MENU_OTAU || Menu.Item == MENU_OTAM){  // OTA Update.
        if (Menu.SubItem == 0){
            O = "MENU to Connect";
        }else if (Menu.SubItem == 1){
            O = "Connecting...";
        }else if (Menu.SubItem == 2 || Menu.SubItem == 3){
            O = WiFi.localIP().toString();
        }
    }else if (Menu.Item == MENU_RSET){  // Reboot Watchy
        O = "MENU to Reboot";
    }
    if (O > ""){
        display.getTextBounds(O, 0, DataY, &x1, &y1, &w, &h);
        w = (196 - w) /2;
        display.setCursor(w + 2, DataY);
        display.print(O);
    }
    Updates.Item = (O != Menu.LastItem);
    Menu.LastItem = O;
}

void WatchyGSR::deepSleep(){
  display.hibernate();
  
  #ifndef ESP_RTC
  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  #endif
  #ifdef ESP_RTC
  esp_sleep_enable_timer_wakeup(60000000);
  #endif
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}

void WatchyGSR::detectBattery(){
    float CBAT, BATOff;
    CBAT = getBatteryVoltage(); // Check battery against previous versions to determine which direction the battery is going.
    BATOff = CBAT - Battery.Last;
     //Detect Power direction

    if (BATOff > 0.03){
      Battery.UpCount++;
      if (Battery.UpCount > 3){
            Battery.Direction = 1; Battery.UpCount = 0; Battery.DownCount = 0; Battery.Last = CBAT; 
            // Check if the NTP has been done.
            if (WatchTime.UTC_RAW - NTPData.Last > 14400 && NTPData.State == 0){
                NTPData.State = 1;
                NTPData.TimeZone = (WatchTime.POSIX[0] == 0);   // No Timezone, go get one!
                NTPData.UpdateUTC = true;
                AskForWiFi();
            }
        }
    }else{
        if (BATOff < 0.00) Battery.DownCount++;
        if (Battery.DownCount > 2){ Battery.Direction = -1; Battery.UpCount = 0; Battery.DownCount = 0; Battery.Last = CBAT; }
    }
}

void WatchyGSR::ProcessNTP(){
  // Do ProgressNTP here.
  switch (NTPData.State){
    // Start WiFi and Connect.
    case 1:{
      HTTP.setUserAgent(UserAgent);
      if (WiFi.status() != WL_CONNECTED){
          if(currentWiFi() == WL_CONNECT_FAILED){
              NTPData.Pause = 0;
              NTPData.State = 99;
              break;
          }
      }
      NTPData.Wait = 0;
      NTPData.Pause = 80;
      NTPData.State++;
      break;
    }

    // Am I Connected?  If so, ask for NTP.
    case 2:{
      if (WiFi.status() != WL_CONNECTED){
          if(currentWiFi() == WL_CONNECT_FAILED){
              NTPData.Pause = 0;
              NTPData.State = 99;
              break;
          }
          NTPData.Pause = 80;
          if (NTPData.Wait > 2){
              NTPData.Pause = 0;
              NTPData.State = 99;
              break;
          }
          break;
      }
      if (NTPData.TimeZone == false){
          NTPData.State = 5;
          NTPData.Pause = 0;
          break;
      }

      NTPData.State++;
      setStatus("TZ");
      // Do the next part.
      HTTP.begin(WiFiC, TZURL);  // Call it and leave.
      NTPData.Wait = 0;
      NTPData.Pause = 80;
      break;
  }

    case 3:{
      // Get GeoLocation
      if (WiFi.status() == WL_DISCONNECTED){
          NTPData.Pause = 0;
          NTPData.State = 99;
          break;
      }
      if (HTTP.GET() == HTTP_CODE_OK) {
        String payload = HTTP.getString();
        JSONVar root = JSON.parse(payload);
        String S = JSON.stringify(root["timezone"]);
        S.replace('"',' ');
        S.trim();
        WatchTime.TimeZone = S;
        HTTP.end();
        NTPData.Wait = 0;
        NTPData.Pause = 0;
        NTPData.State++;   // Test
      }
      break;
    }

    case 4:{
      // Process Timezone POSIX.
      String NewTZ = OP.getPOSIX(WatchTime.TimeZone);
      if(NewTZ == OP.TZMISSING){
          NTPData.Pause = 0;
          NTPData.State = 99;
          break;
      }else if (NewTZ ==""){
          break;
      }
      strcpy(WatchTime.POSIX, NewTZ.c_str());
      NTPData.Wait = 0;
      NTPData.Pause = 0;
      NTPData.State++;
      if (!NTPData.UpdateUTC) UpdateDisp=true;
      break;
    }

    case 5:{
    if (NTPData.UpdateUTC == false || WiFi.status() == WL_DISCONNECTED){
          NTPData.State = 99;
          NTPData.Pause = 0;
          break;
      }
      setStatus("NTP");
      configTime(0,0,"132.246.11.237","132.246.11.227");
      NTPData.Wait = 0;
      NTPData.Pause = 20;
      NTPData.State++;
      break;
    }

    case 6:{
      if (time(nullptr) < 1000000000l){
        if (NTPData.Wait > 2){
            NTPData.Pause = 0;
            NTPData.State = 99;
            break;
        }
        break;
      }
      WatchTime.UTC_RAW = time(nullptr);
      tmElements_t TM;
      breakTime(WatchTime.UTC_RAW,TM);
      RTC.write(TM);
      NTPData.Pause = 0;
      NTPData.State = 99;
      break;
    }

    case 99:{
      OTAEnd = true;
      NTPData.Wait = 0;
      NTPData.Pause = 0;
      NTPData.State = 0;
      NTPData.Last = WatchTime.UTC_RAW; // Moved from section 6 to here, to limit the atttempts.
      NTPData.UpdateUTC = false;
      NTPData.TimeZone = false;
      Battery.UpCount=0;  // Stop it from thinking the battery went wild.
    }
  }
}

void WatchyGSR::drawChargeMe(){
  // Shows Battery Direction indicators.
  int8_t D = 0;
  if (Battery.Direction == 1){
      // Show Battery charging bitmap.
      display.drawBitmap(155, 178, Charging, 40, 17, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
      D = 2;
  }else if (Battery.Direction == -1 && getBatteryVoltage() < MinBattery){
      // Show Battery needs charging bitmap.
      display.drawBitmap(155, 178, ChargeMe, 40, 17, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
      D = 1;
  }
  Updates.Charge = (D != Battery.LastState);
  Battery.LastState = D;
}

void WatchyGSR::drawStatus(){
  if (WatchyStatus > ""){
      display.fillRect(NTPX, NTPY - 19, 60, 20, Options.LightMode ? GxEPD_WHITE : GxEPD_BLACK);
      display.setFont(&Bronova_Regular13pt7b);
      display.setCursor(NTPX + 17, NTPY);
//      display.setTextColor(Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
//      display.print(WatchyStatus);
      if (WatchyStatus.startsWith("WiFi")){
          display.drawBitmap(NTPX, NTPY - 18, iWiFi, 19, 19, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
          display.print(WatchyStatus.substring(4));
      }
      else if (WatchyStatus == "TZ") display.drawBitmap(NTPX, NTPY - 18, iTZ, 19, 19, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
      else if (WatchyStatus == "NTP") display.drawBitmap(NTPX, NTPY - 18, iSync, 19, 19, Options.LightMode ? GxEPD_BLACK : GxEPD_WHITE);
  }
}

void WatchyGSR::setStatus(String Status){
    if (WatchyStatus != Status){
      WatchyStatus = Status;
      Updates.Status=true;
      UpdateDisp=true;
    }
}

void WatchyGSR::VibeTo(bool Mode){
    if (Mode != VibeMode){
        if (Mode){
            sensor.enableFeature(BMA423_WAKEUP, false);
            pinMode(VIB_MOTOR_PIN, OUTPUT);
            digitalWrite(VIB_MOTOR_PIN, true);
        }else{
            digitalWrite(VIB_MOTOR_PIN, false);
            sensor.enableFeature(BMA423_WAKEUP, true);
        }
        VibeMode = Mode;
    }
}

/*
void WatchyGSR::handleAccelerometer(){
  uint8_t Direction;

  //Serial.println("Accel int.");
  if (sensor.isTilt()){
    DBug("Tilt!!!");
    Direction = sensor.getDirection();  // Directional screen on.  == DIRECTION_DISP_UP
    if ((Direction == DIRECTION_DISP_UP) != ScreenOn){
      UpdateDisp = true;  // Update Screen to new state.
    }
  }else if (sensor.isDoubleClick()){
    DBug("Double Tap!");
  }
}*/

void WatchyGSR::handleButtonPress(uint8_t Pressed){
  uint8_t Direction;
  int ml, mh;

  // Check to see if screen is upward, if it is/isn't, make sure it matches.
  Direction = sensor.getDirection();  // Directional screen on.  == DIRECTION_DISP_UP

  /*  Not sure if this Wrist wake is worth the effort or the battery abuse.
  if (wakeupBit & ACC_INT_MASK){  // Movement caused this.
      //Serial.println("Accel int.");
      if ((Direction == DIRECTION_DISP_UP) != ScreenOn){
          UpdateDisp = true;  // Update Screen to new state.
      }
  }
  */

  if (Options.Orientated) { if (Direction != DIRECTION_DISP_UP && Direction != DIRECTION_TOP_EDGE) return; } // Don't accept it.

  switch (Pressed){
    case 1:
          if (GuiMode != MENUON){
            GuiMode = MENUON;
            Menu.LastItem="";
            Menu.LastHeader="";
            DoHaptic = true;
            UpdateDisp = true;  // Quick Update.
            SetTurbo();
          }else if (GuiMode == MENUON){
              if (Menu.Item == MENU_OPTIONS && Menu.SubItem == 0){  // Options
                  Menu.Item = MENU_DISP;
                  Menu.Style = MENU_INOPTIONS;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_STEPS){ // Steps
                  if (Menu.SubItem == 4){
                      sensor.resetStepCounter();
                      Menu.SubItem = 0;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else if (Menu.SubItem < 4){
                      Menu.SubItem++;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }
              }else if (Menu.Item == MENU_ALARMS){  // Alarms menu.
                  Menu.Style = MENU_INALARMS;
                  Menu.Item = MENU_ALARM1;
                  DoHaptic = true;
                  UpdateDisp = true;
                  SetTurbo();
              }else if (Menu.Item >= MENU_ALARM1 && Menu.Item <= MENU_ALARM4){  // Alarms
                  if (Menu.SubItem < 5){
                      Menu.SubItem++;
                      if (Menu.SubItem == 5) Menu.SubItem += WatchTime.Local.Wday; // Jump ahead to the day.
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else if (Menu.SubItem > 4 && Menu.SubItem < 12){
                      Alarms_Active[Menu.Item - MENU_ALARM1] ^= Bits[Menu.SubItem - 5];  // Toggle day.
                      Menu.LastItem=""; // Forces a redraw.
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else if (Menu.SubItem == 12){
                      Alarms_Active[Menu.Item - MENU_ALARM1] ^= ALARM_REPEAT;  // Toggle repeat.
                      Menu.LastItem=""; // Forces a redraw.
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else if (Menu.SubItem == 13){
                      Alarms_Active[Menu.Item - MENU_ALARM1] ^= ALARM_ACTIVE;  // Toggle Active.
                      Menu.LastItem=""; // Forces a redraw.
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }
              }else if (Menu.Item == MENU_TONES){   // Tones.
                      Options.MasterRepeats = clamp(Options.MasterRepeats + 1, 0, 4);
                      Alarms_Repeats[0] = Options.MasterRepeats;
                      Alarms_Repeats[1] = Options.MasterRepeats;
                      Alarms_Repeats[2] = Options.MasterRepeats;
                      Alarms_Repeats[3] = Options.MasterRepeats;
                      Menu.LastItem=""; // Forces a redraw.
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
              }else if (Menu.Item == MENU_TIMERS){  // Timers menu.
                  Menu.Style = MENU_INTIMERS;
                  Menu.Item = MENU_TIMEDN;
                  DoHaptic = true;
                  UpdateDisp = true;
                  SetTurbo();
              }else if (Menu.Item == MENU_TIMEDN){
                  if (Menu.SubItem == 5){
                      if (TimerDown.Active){
                          TimerDown.Active=false;
                          DoHaptic = true;
                          UpdateDisp = true;  // Quick Update.
                          SetTurbo();
                      }else if ((TimerDown.MaxMins + TimerDown.MaxHours) > 0){
                          TimerDown.Mins = TimerDown.MaxMins;
                          TimerDown.Hours = TimerDown.MaxHours;
                          TimerDown.LastUTC = WatchTime.UTC_RAW - WatchTime.UTC.Second;
                          TimerDown.Active = true;
                          DoHaptic = true;
                          UpdateDisp = true;  // Quick Update.
                          SetTurbo();
                      }
                  }else{
                      Menu.SubItem++;
                      if (TimerDown.MaxMins + TimerDown.MaxHours == 0 && Menu.SubItem == 5) Menu.SubItem = 4; //Stop it from being startable.
                      if (TimerDown.Active) Menu.SubItem = 5;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }
              }else if (Menu.Item == MENU_TIMEUP){
                  if (Menu.SubItem == 0){
                      Menu.SubItem = 1;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else{
                      if (TimerUp.Active){
                          TimerUp.StopAt = WatchTime.UTC_RAW - WatchTime.UTC.Second;
                          TimerUp.Active = false;
                      }else{
                          TimerUp.SetAt = WatchTime.UTC_RAW - WatchTime.UTC.Second;
                          TimerUp.StopAt = TimerUp.SetAt;
                          TimerUp.Active = true;
                      }
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }
              }else if (Menu.Item == MENU_SYNC){  // Sync Time
                  if (Menu.SubItem == 0){
                      Menu.SubItem++;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                  }else{
                      NTPData.State = 1;
                      switch (Menu.SubItem){
                        case 1:
                          NTPData.UpdateUTC = true;
                          NTPData.TimeZone = (WatchTime.POSIX[0] == 0);   // No Timezone, go get one!
                          break;
                        case 2:
                          NTPData.UpdateUTC = false;
                          NTPData.TimeZone = true;
                          break;
                        case 3:
                          NTPData.TimeZone = true;
                          NTPData.UpdateUTC = true;
                      }
                      GuiMode = WATCHON;
                      WatchTime.LastDay="";
                      WatchTime.LastDate="";
                      Menu.Item = MENU_DISP;
                      Menu.SubItem = 0;
                      DoHaptic = true;
                      UpdateDisp = true;  // Quick Update.
                      SetTurbo();
                      AskForWiFi();
                  }
              }else if (Menu.Item == MENU_DISP){  // Switch Mode
                  Options.LightMode = !Options.LightMode;
                  Menu.LastItem=""; // Forces a redraw.
                  Updates.Full = true;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_SIDE){  // Dexterity Mode
                  Options.Lefty = !Options.Lefty;
                  Menu.LastItem=""; // Forces a redraw.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_SWAP){  // Swap Menu/Back Buttons
                  Options.Swapped = !Options.Swapped;
                  Menu.LastItem=""; // Forces a redraw.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_BRDR){  // Border Mode
                  Options.Border = !Options.Border;
                  Menu.LastItem=""; // Forces a redraw.
                  display.init(0,false);  // Force it here so it fixes the border.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_ORNT){  // Watchy Orientation
                  Options.Orientated = !Options.Orientated;
                  Menu.LastItem=""; // Forces a redraw.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_MODE){  // Switch Time Mode
                  Options.TwentyFour = !Options.TwentyFour;
                  Menu.LastItem=""; // Forces a redraw.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_FEED){  // Feedback.
                  Options.Feedback = !Options.Feedback;
                  Menu.LastItem=""; // Forces a redraw.
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_TRBO){  // Turbo
                  Options.Turbo = roller(Options.Turbo + 1, 0, 10);
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_SCRN){  // Reset Screen
                  GuiMode = WATCHON;
                  WatchTime.LastDay="";
                  WatchTime.LastDate="";
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  Updates.Full = true;
                  SetTurbo();
              }else if (Menu.Item == MENU_WIFI){  // Watchy Connect
                  Menu.SubItem++;
                  WatchyAPOn = true;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.Item == MENU_OTAU || Menu.Item == MENU_OTAM){  // Watchy OTA
                  Menu.SubItem++;
                  OTAUpdate=true;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  AskForWiFi();
              }else if (Menu.Item == MENU_RSET){  // Watchy Reboot
                  ESP.restart();
              }
          }
          break;
    case 2:
      if (GuiMode == MENUON){   // Back Button [SW2]
          if (Menu.Item == MENU_STEPS && Menu.SubItem > 0) {  // Exit for Steps, back to Steps.
              if (Menu.SubItem == 4) Menu.SubItem = 2;  // Go back to the Hour, so it is the same as the alarms.
              Menu.SubItem--;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item >= MENU_ALARM1 && Menu.Item <= MENU_ALARM4 && Menu.SubItem > 0){
              if (Menu.SubItem < 5 && Menu.SubItem > 0){
                  Menu.SubItem--;
              }else if (Menu.SubItem > 4){
                  Menu.SubItem = 1;
              }
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_TIMEDN && Menu.SubItem > 0){
              Menu.SubItem--;
              if (TimerDown.Active) Menu.SubItem = 0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_TIMEUP && Menu.SubItem > 0){
              Menu.SubItem = 0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_SYNC && Menu.SubItem > 0){
              Menu.SubItem = 0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_WIFI && Menu.SubItem > 0){
              Menu.SubItem = 0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if ((Menu.Item == MENU_OTAU || Menu.Item == MENU_OTAM) && Menu.SubItem > 0){
              break;    // DO NOTHING!
          }else if (Menu.Style == MENU_INALARMS){  // Alarms
              Menu.Style = MENU_INNORMAL;
              Menu.Item = MENU_ALARMS;
              DoHaptic = true;
              UpdateDisp = true;
              SetTurbo();
          }else if (Menu.Style == MENU_INTIMERS){  // Timers
              Menu.Style = MENU_INNORMAL;
              Menu.Item = MENU_TIMERS;
              DoHaptic = true;
              UpdateDisp = true;          
              SetTurbo();
          }else if (Menu.Style == MENU_INOPTIONS){  // Options
              Menu.SubItem = 0;
              Menu.SubSubItem = 0;
              Menu.Item=MENU_OPTIONS;
              Menu.Style=MENU_INNORMAL;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else{
              GuiMode = WATCHON;
              WatchTime.LastDay="";
              WatchTime.LastDate="";
              Menu.SubItem = 0;
              Menu.SubSubItem = 0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }
      }
      break;
    case 3:
      if (GuiMode == MENUON){     // Up Button [SW3]
          // Handle the sideways choices here.
          if (Menu.Item == MENU_STEPS && Menu.SubItem > 0){
              switch (Menu.SubItem){
              case 1: // Hour
                  Steps.Hour=roller(Steps.Hour + 1, 0,23);
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 2: //  x0 Minutes
                  mh = (Steps.Minutes / 10);
                  ml = Steps.Minutes - (mh * 10);
                  mh = roller(mh + 1, 0, 5);
                  Steps.Minutes = (mh * 10) + ml;
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 3: //  x0 Minutes
                  mh = (Steps.Minutes / 10);
                  ml = Steps.Minutes - (mh * 10);
                  ml = roller(ml + 1, 0, 9);
                  Steps.Minutes = (mh * 10) + ml;
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          }else if (Menu.Item >= MENU_ALARM1 && Menu.Item <= MENU_ALARM4 && Menu.SubItem > 0){
              if (Menu.SubItem == 1){ // Hour
                  Alarms_Hour[Menu.Item - MENU_ALARM1]=roller(Alarms_Hour[Menu.Item - MENU_ALARM1] + 1, 0,23);
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem == 2){ //  x0 Minutes
                  mh = (Alarms_Minutes[Menu.Item - MENU_ALARM1] / 10);
                  ml = Alarms_Minutes[Menu.Item - MENU_ALARM1] - (mh * 10);
                  mh = roller(mh + 1, 0, 5);
                  Alarms_Minutes[Menu.Item - MENU_ALARM1] = (mh * 10) + ml;
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem == 3){ //  x0 Minutes
                  mh = (Alarms_Minutes[Menu.Item - MENU_ALARM1] / 10);
                  ml = Alarms_Minutes[Menu.Item - MENU_ALARM1] - (mh * 10);
                  ml = roller(ml + 1, 0, 9);
                  Alarms_Minutes[Menu.Item - MENU_ALARM1] = (mh * 10) + ml;
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem == 4){ //  Repeats.
                  Alarms_Repeats[Menu.Item - MENU_ALARM1] = roller(Alarms_Repeats[Menu.Item - MENU_ALARM1] - 1, 0, 4);
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem > 4){
                  Menu.SubItem = roller(Menu.SubItem + 1, 5, 13);
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          } else if (Menu.Item == MENU_TIMEDN && Menu.SubItem > 0){
              switch (Menu.SubItem){
              case 1: // Hour
                  TimerDown.MaxHours=roller(TimerDown.MaxHours + 1, 0,23);
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 2: //  x0 Minutes
                  mh = (TimerDown.MaxMins / 10);
                  ml = TimerDown.MaxMins - (mh * 10);
                  mh = roller(mh + 1, 0, 5);
                  TimerDown.MaxMins = (mh * 10) + ml;
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 3: //  x0 Minutes
                  mh = (TimerDown.MaxMins / 10);
                  ml = TimerDown.MaxMins - (mh * 10);
                  ml = roller(ml + 1, 0, 9);
                  TimerDown.MaxMins = (mh * 10) + ml;
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 4:   //Repeats
                  TimerDown.MaxTones = roller(TimerDown.MaxTones - 1, 0, 4);
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          }else if (Menu.Item == MENU_SYNC && Menu.SubItem > 0){
              Menu.SubItem = roller(Menu.SubItem - 1, 1, 3);
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_WIFI && Menu.SubItem > 0){
              // Do nothing!
          }else{
              if (Menu.Style == MENU_INOPTIONS){
                  Menu.Item = roller(Menu.Item - 1, MENU_DISP, (NTPData.State > 0 || WatchyAPOn || OTAUpdate || getBatteryVoltage() < MinBattery) ? MENU_SCRN : MENU_RSET);
              }else if (Menu.Style == MENU_INALARMS){
                  Menu.Item = roller(Menu.Item - 1, MENU_ALARM1, MENU_TONES);
              }else if (Menu.Style == MENU_INTIMERS){
                  Menu.Item = roller(Menu.Item - 1, MENU_TIMEDN, MENU_TIMEUP);
              }else{
                  Menu.Item = roller(Menu.Item - 1, MENU_STEPS, MENU_OPTIONS);
              }
              Menu.SubItem=0;
              Menu.SubSubItem=0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }
      }
      break;
    case 4:
      if (GuiMode == MENUON){   // Down Button [SW4]
          // Handle the sideways choices here.
          if (Menu.Item == MENU_STEPS && Menu.SubItem > 0){
              switch (Menu.SubItem){
              case 1: // Hour
                  Steps.Hour=roller(Steps.Hour - 1, 0,23);
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 2: //  x0 Minutes
                  mh = (Steps.Minutes / 10);
                  ml = Steps.Minutes - (mh * 10);
                  mh = roller(mh - 1, 0, 5);
                  Steps.Minutes = (mh * 10) + ml;
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 3: //  x0 Minutes
                  mh = (Steps.Minutes / 10);
                  ml = Steps.Minutes - (mh * 10);
                  ml = roller(ml - 1, 0, 9);
                  Steps.Minutes = (mh * 10) + ml;
                  Steps.Reset = false;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          }else if (Menu.Item >= MENU_ALARM1 && Menu.Item <= MENU_ALARM4 && Menu.SubItem > 0){
              if (Menu.SubItem == 1){ // Hour
                  Alarms_Hour[Menu.Item - MENU_ALARM1]=roller(Alarms_Hour[Menu.Item - MENU_ALARM1] - 1, 0,23);
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
             }else if (Menu.SubItem == 2){ //  x0 Minutes
                  mh = (Alarms_Minutes[Menu.Item - MENU_ALARM1] / 10);
                  ml = Alarms_Minutes[Menu.Item - MENU_ALARM1] - (mh * 10);
                  mh = roller(mh - 1, 0, 5);
                  Alarms_Minutes[Menu.Item - MENU_ALARM1] = (mh * 10) + ml;
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem == 3){ //  x0 Minutes
                  mh = (Alarms_Minutes[Menu.Item - MENU_ALARM1] / 10);
                  ml = Alarms_Minutes[Menu.Item - MENU_ALARM1] - (mh * 10);
                  ml = roller(ml - 1, 0, 9);
                  Alarms_Minutes[Menu.Item - MENU_ALARM1] = (mh * 10) + ml;
                  Alarms_Active[Menu.Item - MENU_ALARM1] &= ALARM_NOTRIGGER;
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem == 4){ //  Repeats.
                  Alarms_Repeats[Menu.Item - MENU_ALARM1] = roller(Alarms_Repeats[Menu.Item - MENU_ALARM1] + 1, 0, 4);
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }else if (Menu.SubItem > 4){
                  Menu.SubItem = roller(Menu.SubItem - 1, 5, 13);
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          } else if (Menu.Item == MENU_TIMEDN && Menu.SubItem > 0){
              switch (Menu.SubItem){
              case 1: // Hour
                  TimerDown.MaxHours=roller(TimerDown.MaxHours - 1, 0,23);
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 2: //  x0 Minutes
                  mh = (TimerDown.MaxMins / 10);
                  ml = TimerDown.MaxMins - (mh * 10);
                  mh = roller(mh - 1, 0, 5);
                  TimerDown.MaxMins = (mh * 10) + ml;
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 3: //  x0 Minutes
                  mh = (TimerDown.MaxMins / 10);
                  ml = TimerDown.MaxMins - (mh * 10);
                  ml = roller(ml - 1, 0, 9);
                  TimerDown.MaxMins = (mh * 10) + ml;
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
                  break;
              case 4:   //Repeats
                  TimerDown.MaxTones = roller(TimerDown.MaxTones + 1, 0, 4);
                  StopCD();
                  DoHaptic = true;
                  UpdateDisp = true;  // Quick Update.
                  SetTurbo();
              }
          }else if (Menu.Item == MENU_SYNC && Menu.SubItem > 0){
              Menu.SubItem = roller(Menu.SubItem + 1, 1, 3);
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }else if (Menu.Item == MENU_WIFI && Menu.SubItem > 0){
              // Do nothing!
          }else{
              if (Menu.Style == MENU_INOPTIONS){
                  Menu.Item = roller(Menu.Item + 1, MENU_DISP, (NTPData.State > 0 || WatchyAPOn || OTAUpdate || getBatteryVoltage() < MinBattery) ? MENU_SCRN : MENU_RSET);
              }else if (Menu.Style == MENU_INALARMS){
                  Menu.Item = roller(Menu.Item + 1, MENU_ALARM1, MENU_TONES);
              }else if (Menu.Style == MENU_INTIMERS){
                  Menu.Item = roller(Menu.Item + 1, MENU_TIMEDN, MENU_TIMEUP);
              }else{
                  Menu.Item = roller(Menu.Item + 1, MENU_STEPS, MENU_OPTIONS);
              }
              Menu.SubItem=0;
              Menu.SubSubItem=0;
              DoHaptic = true;
              UpdateDisp = true;  // Quick Update.
              SetTurbo();
          }
      }
  }
}

void WatchyGSR::UpdateUTC(){
    tmElements_t TM;  //    struct tm * tm;
    RTC.read(TM);
    WatchTime.UTC = TM;
    WatchTime.UTC_RAW = makeTime(TM);
}

void WatchyGSR::UpdateClock(){
    struct tm * tm;
    setenv("TZ",WatchTime.POSIX,1);
    tzset();
    tm = localtime(&WatchTime.UTC_RAW);
    WatchTime.Local.Second = tm->tm_sec;
    WatchTime.Local.Minute = tm->tm_min;
    WatchTime.Local.Hour = tm->tm_hour;
    WatchTime.Local.Wday = tm->tm_wday;
    WatchTime.Local.Day = tm->tm_mday;
    WatchTime.Local.Month = tm->tm_mon + 1;
    WatchTime.Local.Year = tm->tm_year;
}

void WatchyGSR::_rtcConfig() {
  tmElements_t TM;
  //https://github.com/JChristensen/DS3232RTC
  RTC.squareWave(SQWAVE_NONE); //disable square wave output
  //RTC.set(compileTime()); //set RTC time to compile time
  RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
  RTC.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
  RTC.read(TM);
}

void WatchyGSR::_bmaConfig() {

  if (sensor.begin(_readRegister, _writeRegister, delay) == false) {
    //fail to init BMA
    return;
  }

  // Accel parameter structure
  Acfg cfg;
  /*!
      Output data rate in Hz, Optional parameters:
          - BMA4_OUTPUT_DATA_RATE_0_78HZ
          - BMA4_OUTPUT_DATA_RATE_1_56HZ
          - BMA4_OUTPUT_DATA_RATE_3_12HZ
          - BMA4_OUTPUT_DATA_RATE_6_25HZ
          - BMA4_OUTPUT_DATA_RATE_12_5HZ
          - BMA4_OUTPUT_DATA_RATE_25HZ
          - BMA4_OUTPUT_DATA_RATE_50HZ
          - BMA4_OUTPUT_DATA_RATE_100HZ
          - BMA4_OUTPUT_DATA_RATE_200HZ
          - BMA4_OUTPUT_DATA_RATE_400HZ
          - BMA4_OUTPUT_DATA_RATE_800HZ
          - BMA4_OUTPUT_DATA_RATE_1600HZ
  */
  cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  /*!
      G-range, Optional parameters:
          - BMA4_ACCEL_RANGE_2G
          - BMA4_ACCEL_RANGE_4G
          - BMA4_ACCEL_RANGE_8G
          - BMA4_ACCEL_RANGE_16G
  */
  cfg.range = BMA4_ACCEL_RANGE_2G;
  /*!
      Bandwidth parameter, determines filter configuration, Optional parameters:
          - BMA4_ACCEL_OSR4_AVG1
          - BMA4_ACCEL_OSR2_AVG2
          - BMA4_ACCEL_NORMAL_AVG4
          - BMA4_ACCEL_CIC_AVG8
          - BMA4_ACCEL_RES_AVG16
          - BMA4_ACCEL_RES_AVG32
          - BMA4_ACCEL_RES_AVG64
          - BMA4_ACCEL_RES_AVG128
  */
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

  /*! Filter performance mode , Optional parameters:
      - BMA4_CIC_AVG_MODE
      - BMA4_CONTINUOUS_MODE
  */
  cfg.perf_mode = BMA4_CONTINUOUS_MODE;

  // Configure the BMA423 accelerometer
  sensor.setAccelConfig(cfg);

  // Enable BMA423 accelerometer
  // Warning : Need to use feature, you must first enable the accelerometer
  sensor.enableAccel();

  struct bma4_int_pin_config config ;
  config.edge_ctrl = BMA4_LEVEL_TRIGGER;
  config.lvl = BMA4_ACTIVE_HIGH;
  config.od = BMA4_PUSH_PULL;
  config.output_en = BMA4_OUTPUT_ENABLE;
  config.input_en = BMA4_INPUT_DISABLE;
  // The correct trigger interrupt needs to be configured as needed
  sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

  struct bma423_axes_remap remap_data;
  remap_data.x_axis = 1;
  remap_data.x_axis_sign = 0xFF;
  remap_data.y_axis = 0;
  remap_data.y_axis_sign = 0xFF;
  remap_data.z_axis = 2;
  remap_data.z_axis_sign = 0xFF;
  // Need to raise the wrist function, need to set the correct axis
  sensor.setRemapAxes(&remap_data);

  // Enable BMA423 isStepCounter feature
  sensor.enableFeature(BMA423_STEP_CNTR, true);
  // Enable BMA423 isTilt feature
  //sensor.enableFeature(BMA423_TILT, true);
  // Enable BMA423 isDoubleClick feature
  //sensor.enableFeature(BMA423_WAKEUP, true);

  // Reset steps
  //sensor.resetStepCounter();

  // Turn on feature interrupt
  //sensor.enableStepCountInterrupt();
  //sensor.enableTiltInterrupt();
  // It corresponds to isDoubleClick interrupt
  //sensor.enableWakeupInterrupt();
}

float WatchyGSR::getBatteryVoltage(){
    float A, B, C, D;
    A = analogRead(ADC_PIN) - 0.0125;
    B = analogRead(ADC_PIN) - 0.0125;
    C = analogRead(ADC_PIN) - 0.0125;
    D = analogRead(ADC_PIN) - 0.0125;
    return (((A + B + C + D) / 16384.0) * 7.23);
}

uint16_t WatchyGSR::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)address, (uint8_t)len);
  uint8_t i = 0;
  while (Wire.available()) {
    data[i++] = Wire.read();
  }
  return 0;
}

uint16_t WatchyGSR::_writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data, len);
  return (0 !=  Wire.endTransmission());
}

String WatchyGSR::MakeTime(int Hour, int Minutes, bool& Alarm) {  // Use variable with Alarm, if set to False on the way in, returns PM indication.
    int H;
    String AP;
    H = Hour;
    if (!Options.TwentyFour){
        if (H > 11){
          AP = " PM";
          if (!Alarm){
              Alarm = true;   // Tell the clock to use the PM indicator.
          }
      }else{
          AP = " AM";
        }
        if (H > 12){
            H -= 12;
        }else if (H == 0){
            H = 12;
        }
    }
    return String(H) + (Minutes < 10 ? ":0" : ":") + String(Minutes) + (!Options.TwentyFour ? AP : "");
}

String WatchyGSR::MakeHour(uint8_t Hour){
    int H;
    H = Hour;
    if (!Options.TwentyFour){
        if (H > 12){
          H -= 12;
        }
        if (H == 0){
          H = 12;
        }
    }
    return String(H);
}

String WatchyGSR::MakeMinutes(uint8_t Minutes){
    return (Minutes < 10 ? "0" : "") + String(Minutes);
}

String WatchyGSR::MakeSteps(uint32_t uSteps){
    String S, T, X;
    int I, C;

    S = String(uSteps);
    I = S.length();
    C = 0; T = "";

    while (I > 0){
        X = (I > 1 && C == 2) ? "," : "";
        T = X + S.charAt(I - 1) + T;
        C = roller(C + 1, 0, 2); I--;
    }
    return T;
}

void WatchyGSR::CheckAlarm(int I){
    uint16_t  B;
    bool bA;
    B = (ALARM_ACTIVE | Bits[WatchTime.Local.Wday]);
    bA = (Alarms_Hour[I] == WatchTime.Local.Hour && Alarms_Minutes[I] == WatchTime.Local.Minute);
    if (!bA && Alarms_Times[I] == 0 && (Alarms_Active[I] & ALARM_TRIGGERED) != 0){
        Alarms_Active[I] &= ALARM_NOTRIGGER;
    }else if ((Alarms_Active[I] & B) == B){  // Active and Active Day.
        if (bA && Alarms_Times[I] == 0 && (Alarms_Active[I] & ALARM_TRIGGERED) == 0){
            Alarms_Times[I] = 255;
            Alarms_Playing[I] = 30;
            Alarms_Active[I] |= ALARM_TRIGGERED;
            if ((Alarms_Active[I] & ALARM_REPEAT) == 0){
                Alarms_Active[I] &= (ALARM_ALL - Bits[WatchTime.Local.Wday]);
                if ((Alarms_Active[I] & ALARM_DAYS) == 0){
                    Alarms_Active[I] ^= ALARM_ACTIVE; // Turn it off, not repeating.
                }
            }
        }
    }
}

void WatchyGSR::CheckCD(){
  uint16_t M = ((WatchTime.UTC_RAW - TimerDown.LastUTC) / 60);
  uint16_t E;

    if ( M > 0){
        TimerDown.LastUTC = WatchTime.UTC_RAW;
        E = TimerDown.Mins + (TimerDown.Hours * 60) - M;
        TimerDown.Hours = (E / 60);
        TimerDown.Mins = E - (TimerDown.Hours * 60);
    }
    if (TimerDown.Hours == 0 && TimerDown.Mins == 0 && TimerDown.Active){
        TimerDown.Tone = 24;
        TimerDown.ToneLeft = 255;
        TimerDown.Active = false;
        UpdateDisp = true;  // Quick Update.
    }
}

// Counts the active (255) alarms/timers and after 3, sets them to lower values.
void WatchyGSR::CalculateTones(){
    uint8_t Count = 0;
    CheckAlarm(0); CheckAlarm(1); CheckAlarm(2); CheckAlarm(3); CheckCD();
    if (Alarms_Times[0] > 0) Count++;
    if (Alarms_Times[1] > 0) Count++;
    if (Alarms_Times[2] > 0) Count++;
    if (Alarms_Times[3] > 0) Count++;
    if (TimerDown.ToneLeft > 0) Count++;
    if (Count == 5){
        if (Alarms_Times[0] == 255) Alarms_Times[0] = 7 * Reduce[Alarms_Repeats[0]];
        if (Alarms_Times[1] == 255) Alarms_Times[1] = 7 * Reduce[Alarms_Repeats[1]];
        if (Alarms_Times[2] == 255) Alarms_Times[2] = 7 * Reduce[Alarms_Repeats[2]];
        if (Alarms_Times[3] == 255) Alarms_Times[3] = 7 * Reduce[Alarms_Repeats[3]];
        if (TimerDown.ToneLeft == 255) TimerDown.ToneLeft = 10 * Reduce[TimerDown.MaxTones];
    }else if (Count == 4){
        if (Alarms_Times[0] == 255) Alarms_Times[0] = 8 * Reduce[Alarms_Repeats[0]];
        if (Alarms_Times[1] == 255) Alarms_Times[1] = 8 * Reduce[Alarms_Repeats[1]];
        if (Alarms_Times[2] == 255) Alarms_Times[2] = 8 * Reduce[Alarms_Repeats[2]];
        if (Alarms_Times[3] == 255) Alarms_Times[3] = 8 * Reduce[Alarms_Repeats[3]];
        if (TimerDown.ToneLeft == 255) TimerDown.ToneLeft = 12 * Reduce[TimerDown.MaxTones];
    }else{
        if (Alarms_Times[0] == 255) Alarms_Times[0] = 10 * Reduce[Alarms_Repeats[0]];
        if (Alarms_Times[1] == 255) Alarms_Times[1] = 10 * Reduce[Alarms_Repeats[1]];
        if (Alarms_Times[2] == 255) Alarms_Times[2] = 10 * Reduce[Alarms_Repeats[2]];
        if (Alarms_Times[3] == 255) Alarms_Times[3] = 10 * Reduce[Alarms_Repeats[3]];
        if (TimerDown.ToneLeft == 255) TimerDown.ToneLeft = 15 * Reduce[TimerDown.MaxTones];
    }
}

void WatchyGSR::StopCD(){
    if (TimerDown.ToneLeft > 0){
        TimerDown.ToneLeft = 1;
        TimerDown.Tone = 1;
    }
}

uint8_t WatchyGSR::getToneTimes(uint8_t ToneIndex){
    if (ToneIndex > 3) return TimerDown.ToneLeft;
    return Alarms_Times[ToneIndex];
}

String WatchyGSR::getReduce(uint8_t Amount){
    switch (Amount){
        case 0:
          return "Full";
        case 1:
          return "80%";
        case 2:
          return "60%";
        case 3:
          return "40%";
    }
    return "20%";
}

// Catches Steps and moves "Yesterday" into the other setting.
void WatchyGSR::monitorSteps(){
    if (Steps.Hour == WatchTime.Local.Hour && Steps.Minutes == WatchTime.Local.Minute){
        if (!Steps.Reset){
            Steps.Yesterday=sensor.getCounter();
            sensor.resetStepCounter();
            Steps.Reset=true;
        }
    }else if (Steps.Reset) Steps.Reset=false;
}

/*
String WatchyGSR::GetStoredTimezone(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    if (nvs_open("Clock",NVS_READWRITE,hNVS) == ESP_OK){
        size_t S;
        nvs_get_str(hNVS, "TimeZone", NULL, &S);
        if (S > 0){
            char* TZ = malloc(S);
            nvs_get_str(hNVS, "TimeZone", TZ, &S);
            nvs_close(hNVS);
            return TZ;
        }
        nvs_close(hNVS);
    }
    return TZMISSING;
}

bool WatchyGSR::StoreTimezone(String Timezone){
    bool B;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    if (nvs_open("Clock",NVS_READWRITE,hNVS) == ESP_OK){
        B = (nvs_set_str(hNVS,"TimeZone",Timezone) == ESP_OK);
        if (B){
            nvs_commit(hNVS);
        }
        nvs_close(hNVS);
    }else{
        return false;
    }
}
*/

IRAM_ATTR void WatchyGSR::handleInterrupt(){
    uint8_t B = getButtonPins();
    if (B > 0 && (millis() - LastButton) > KEYPAUSE) { Button = B; LastButton = millis(); }
}

IRAM_ATTR uint8_t WatchyGSR::getButtonPins(){
    bool SW1 = (digitalRead(MENU_BTN_PIN) == 1);
    bool SW2 = (digitalRead(BACK_BTN_PIN) == 1);
    bool SW3 = (digitalRead(UP_BTN_PIN) == 1);
    bool SW4 = (digitalRead(DOWN_BTN_PIN) == 1);

    if (SW1)      return Options.Lefty ? 4 : getSwapped(1);
    else if (SW2) return Options.Lefty ? 3 : getSwapped(2);
    else if (SW3) return Options.Lefty ? getSwapped(2) : 3;
    else if (SW4) return Options.Lefty ? getSwapped(1) : 4;
    return 0;
}

uint8_t WatchyGSR::getButtonMaskToID(uint64_t HW){
      if (HW & MENU_BTN_MASK)      return Options.Lefty ? 4 : getSwapped(1);   // Menu Button [SW1]
      else if (HW & BACK_BTN_MASK) return Options.Lefty ? 3 : getSwapped(2);   // Back Button [SW2]
      else if (HW & UP_BTN_MASK)   return Options.Lefty ? getSwapped(2) : 3;     // Up Button [SW3]
      else if (HW & DOWN_BTN_MASK) return Options.Lefty ? getSwapped(1) : 4;   // Down Button [SW4]
   return 0;
}

IRAM_ATTR uint8_t WatchyGSR::getSwapped(uint8_t pIn){
    switch (pIn){
        case 1:
          return Options.Swapped ? 2 : 1;
        case 2:
          return Options.Swapped ? 1 : 2;
    }
    return 0;
}

void WatchyGSR::ScreenRefresh(){
    uint16_t XL, YL, XH, YH;
    bool DoIt;

    XL = 200; YL = 200; XH = 0; YH = 0;

    if (!Updates.Full){
        DoIt = false;
        if (Updates.Time)   { XL = 0; YL = TimeY - 45; XH = 200; YH = TimeY; DoIt = true; }
        if (Updates.Day)    { XL = 0; YL = golow(DayY - 16, YL); XH = 200; YH = gobig(DayY, YH); DoIt = true; }
        if (Updates.Date)   { XL = 0; YL = golow(DateY - 16,YL); XH = 200; YH = gobig(DateY, YH); DoIt = true; }
        if (Updates.Header) { XL = 0; YL = golow(MenuTop, YL); XH = 200; YH = gobig(MenuTop + (MenuHeight / 2), YH); DoIt = true; }
        if (Updates.Item)   { XL = 0; YL = golow(MenuTop + (MenuHeight / 2), YL); XH = 200; YH = gobig(MenuTop + (MenuHeight / 2), YH); DoIt = true; }
        if (Updates.Status) { XL = golow(NTPX, XL); YL = golow(NTPY - 19, YL); XH = gobig(60, XH); YH = gobig(NTPY, YH); DoIt = true; }
        if (Updates.Year)   { XL = 0; YL = golow(YearY - 17, YL); XH = gobig(154, XH); YH = gobig(YearY, YH); DoIt = true; }
        if (Updates.Charge) { XL = golow(155, XL); YL = golow(178, YL); XH = gobig(196, XH); YH = gobig(196, YH); DoIt = true; }
    }else{ XL = 0; YL = 0; XH = 200; YH = 200; DoIt = true; }
    if (DoIt){
        if(Updates.Full) display.setFullWindow(); else display.setFullWindow(); //init moved, can't do this:  display.setPartialWindow(XL, YL, XH - XL, YH - YL);
        display.display(!Updates.Full); //partial refresh
    }
    Updates.Time=false;
    Updates.Day=false;
    Updates.Date=false;
    Updates.Header=false;
    Updates.Item=false;
    Updates.Status=false;
    Updates.Year=false;
    Updates.Charge=false;
    Updates.Full=false;
}

void WatchyGSR::AskForWiFi(){ if (!GSRWiFi.Requested && !GSRWiFi.Working) GSRWiFi.Requested = true; }

void WatchyGSR::processWiFiRequest(){
    wl_status_t WiFiE = WL_CONNECT_FAILED;
    wl_status_t rWiFi = WiFi.status();
    String AP, PA;

    if (GSRWiFi.Requested){
        GSRWiFi.Requested = false;
        WiFi.disconnect();
        WiFi.setHostname(WiFi_AP_SSID);
        WiFi.mode(WIFI_STA);
        GSRWiFi.Working = true;
        GSRWiFi.Index = 0;
        GSRWiFi.Last = 0;
    }

    if (GSRWiFi.Working) {
        if (rWiFi == WL_CONNECTED) { GSRWiFi.Working = false; GSRWiFi.Results = true; return; } // We got connected.
        if (GSRWiFi.Last == 0 || millis() - GSRWiFi.Last > 30000){
            if (GSRWiFi.Index < 11){
                if (GSRWiFi.Last != 0) GSRWiFi.Index++;
                GSRWiFi.Last = millis();
                if (GSRWiFi.Index == 0){
                    setStatus("WiFi-X");
                    WiFi.onEvent(WatchyGSR::WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
                    if (WiFi_DEF_SSID > "") WiFiE = WiFi.begin(WiFi_DEF_SSID,WiFi_DEF_PASS); else WiFiE = WiFi.begin();
                    if (WiFiE == WL_CONNECT_FAILED) { GSRWiFi.Last = 0; GSRWiFi.Index++; }   // Try the next one instantly.
                }
                if (GSRWiFi.Index > 0){
                    AP = APIDtoString(GSRWiFi.Index - 1); PA = PASStoString(GSRWiFi.Index - 1);
                    if (AP > ""){
                        setStatus("WiFi-" + char(64 + GSRWiFi.Index));
                        WiFi.disconnect();
                        WiFi.onEvent(WatchyGSR::WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
                        WiFiE = WiFi.begin(AP.c_str(),PA.c_str());
                    }
                    if (WiFiE == WL_CONNECT_FAILED) { GSRWiFi.Last = 0; GSRWiFi.Index++; }   // Try the next one instantly.
                }
            }else endWiFi();
        }
    }
}

wl_status_t WatchyGSR::currentWiFi(){
    if (WiFi.status() == WL_CONNECTED) return WL_CONNECTED;
    if (GSRWiFi.Working) return WL_IDLE_STATUS;  // Make like it is relaxing doing nothing.
    return WL_CONNECT_FAILED;
}

void WatchyGSR::endWiFi(){
    GSRWiFi.Requested = false;
    GSRWiFi.Working = false;
    GSRWiFi.Results = false;
    GSRWiFi.Index = 0;
    setStatus("");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
}

void WatchyGSR::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    GSRWiFi.Last = 0;
}

String WatchyGSR::buildWiFiAPPage(){
    String S = wifiIndexA;
    String T;
    int I = 0;

    while (I < 10){
        T = wifiIndexB;
        T.replace("$",String(char(65 + I)));
        T.replace("#",String(char(48 + I)));
        T.replace("?",APIDtoString(I));
        S += T;

        T = wifiIndexC;
        T.replace("#",String(char(48 + I)));
        T.replace("$",PASStoString(I));
        S += (T + (I < 9 ? "</tr>" : ""));
        I++;
    }
    return S + wifiIndexD;
}

void WatchyGSR::parseWiFiPageArg(String ARG, String DATA){
    uint8_t I = String(ARG.charAt(2)).toInt();
    String S = ARG.substring(0,2);

    if (S == "AP") strcpy(GSRWiFi.AP[I].APID, DATA.c_str());
    if (S == "PA") strcpy(GSRWiFi.AP[I].PASS, DATA.c_str());
}

String WatchyGSR::APIDtoString(uint8_t Index){
    String S = "";
    uint8_t I = 0;
    while (GSRWiFi.AP[Index].APID[I] != 0 && I < 32) { S += char(GSRWiFi.AP[Index].APID[I]); I++; }
    return S;
}

String WatchyGSR::PASStoString(uint8_t Index){
    String S = "";
    uint8_t I = 0;
    while (GSRWiFi.AP[Index].PASS[I] != 0 && I < 63) { S += char(GSRWiFi.AP[Index].PASS[I]); I++; }
    return S;
}

void WatchyGSR::initZeros(){
    String S = "";
    GuiMode = WATCHON;
    ScreenOn = true;
    VibeMode = 0;
    WatchyStatus = "";
    WatchTime.TimeZone = "";
    Menu.Style = MENU_INNORMAL;
    Menu.Item = 0;
    Menu.SubItem = 0;
    Menu.SubSubItem = 0;
    NTPData.Pause = 0;
    NTPData.Wait = 0;
    Battery.Last = getBatteryVoltage() + 1;   // Done for power spike after reboot from making the watch think it's charging.
    Battery.UpCount = 0;
    Battery.DownCount = 0;
    ActiveMode = false;
    OTATry = 0;
    OTAEnd = false;
    OTAUpdate = false;
    OTATimer = millis();
    WatchyAPOn = false;
    DoHaptic = false;
    Steps.Reset=false;
    Alarms_Active[0]=0;
    Alarms_Active[1]=0;
    Alarms_Active[2]=0;
    Alarms_Active[3]=0;
    Alarms_Times[0]=0;
    Alarms_Times[1]=0;
    Alarms_Times[2]=0;
    Alarms_Times[3]=0;
    Alarms_Playing[0]=0;
    Alarms_Playing[1]=0;
    Alarms_Playing[2]=0;
    Alarms_Playing[3]=0;
    Alarms_Repeats[0]=0;
    Alarms_Repeats[1]=0;
    Alarms_Repeats[2]=0;
    Alarms_Repeats[3]=0;
    GSRWiFi.Requested=false;
    GSRWiFi.Working=false;
    GSRWiFi.Results=false;
    GSRWiFi.Index=0;
    strcpy(GSRWiFi.AP[0].APID,S.c_str());
    strcpy(GSRWiFi.AP[0].PASS,S.c_str());
    strcpy(GSRWiFi.AP[1].APID,S.c_str());
    strcpy(GSRWiFi.AP[1].PASS,S.c_str());
    strcpy(GSRWiFi.AP[2].APID,S.c_str());
    strcpy(GSRWiFi.AP[2].PASS,S.c_str());
    strcpy(GSRWiFi.AP[3].APID,S.c_str());
    strcpy(GSRWiFi.AP[3].PASS,S.c_str());
    strcpy(GSRWiFi.AP[4].APID,S.c_str());
    strcpy(GSRWiFi.AP[4].PASS,S.c_str());
    strcpy(GSRWiFi.AP[5].APID,S.c_str());
    strcpy(GSRWiFi.AP[5].PASS,S.c_str());
    strcpy(GSRWiFi.AP[6].APID,S.c_str());
    strcpy(GSRWiFi.AP[6].PASS,S.c_str());
    strcpy(GSRWiFi.AP[7].APID,S.c_str());
    strcpy(GSRWiFi.AP[7].PASS,S.c_str());
    strcpy(GSRWiFi.AP[8].APID,S.c_str());
    strcpy(GSRWiFi.AP[8].PASS,S.c_str());
    strcpy(GSRWiFi.AP[9].APID,S.c_str());
    strcpy(GSRWiFi.AP[9].PASS,S.c_str());
    NTPData.TimeZone=true;
    NTPData.UpdateUTC=true;
    NTPData.State=1;
    TimerUp.SetAt = WatchTime.UTC_RAW;
    TimerUp.StopAt = TimerUp.SetAt;
    TimerDown.MaxMins = 0;
    TimerDown.MaxHours = 0;
    TimerDown.Mins = 0;
    TimerDown.Hours = 0;
    TimerDown.MaxTones = 0;
    TimerDown.Active = false;
    AskForWiFi();
}

// Settings Storage & Retrieval here.

String WatchyGSR::GetSettings(){
    unsigned char I[2048];
    unsigned char O[2048];
    int K = 0;
    int J = 6;
    uint8_t X, Y, W;
    uint16_t V;
    String S;
    size_t L;

       // Retrieve the settings from the current state into a base64 string.

    I[0] = (Steps.Hour);
    I[1] = (Steps.Minutes);
    K  = Options.TwentyFour ? 1 : 0;
    K |= Options.LightMode ? 2 : 0;
    K |= Options.Feedback ? 4 : 0;
    K |= Options.Border ? 8 : 0;
    K |= Options.Lefty ? 16 : 0;
    K |= Options.Swapped ? 32 : 0;
    K |= Options.Orientated ? 64 : 0;
    I[2] = (K); V = (Options.MasterRepeats << 5); I[3] = (Options.Turbo | V);

    V = (TimerDown.MaxTones << 5);
    I[4] = ((TimerDown.MaxHours) | V);
    I[5] = (TimerDown.MaxMins);

    for (K = 0; K < 4; K++){
        V = (Alarms_Repeats[K] << 5);
        I[J] = (Alarms_Hour[K] | V); J++;
        I[J] = (Alarms_Minutes[K]); J++;
        V = (Alarms_Active[K] & ALARM_NOTRIGGER);
        I[J] = (V & 255); J++;
        I[J] = ((V >> 8) & 255); J++;
    }

    for (X = 0; X < 10; X++){
        S = APIDtoString(X);
        if (S > "") {
            W = S.length();
            I[J] = W; J++;
            for (Y = 0; Y < W; Y++){
                I[J] = S.charAt(Y); J++;
            }
            S = PASStoString(X);
            W = S.length();
            I[J] = W; J++;
            for (Y = 0; Y < W; Y++){
                I[J] = S.charAt(Y); J++;
            }
        }
    }
    I[J] = 0; J++;

    mbedtls_base64_encode(&O[0], 2047, &L, &I[0], J);

    O[L]=0;
    S = reinterpret_cast<const char *>(O);
    return S;
}

void WatchyGSR::StoreSettings(String FromUser){
    unsigned char O[2048], E[2048];
    int K = 0;
    int J = 0;
    uint16_t V;
    size_t L;
    bool Ok;
    uint8_t I, A, W;  // For WiFi storage.
    String S;

    J = FromUser.length(); for (K = 0; K < J; K++) E[K] = FromUser.charAt(K);

    mbedtls_base64_decode(&O[0], 2047, &L, &E[0], J); L--;  // Take dead zero off end.

    J = 0; if (L > J) Steps.Hour = clamp(O[J],0,23);
    J++;   if (L > J) Steps.Minutes = clamp(O[J],0,59);
    J++;
    if (L > J) {
        V = O[J];
        Options.TwentyFour = (V & 1) ? true : false;
        Options.LightMode = (V & 2) ? true : false;
        Options.Feedback = (V & 4) ? true : false;
        Options.Border = (V & 8) ? true : false;
        Options.Lefty = (V & 16) ? true : false;
        Options.Swapped = (V & 32) ? true : false;
        Options.Orientated = (V & 64) ? true : false;
    }
    J++; if (L > J){
        V = ((O[J] & 224) >> 5);
        Options.MasterRepeats = V;
        Options.Turbo = clamp((O[J] & 31),0,10);
    }
    J++; if (L > J){
        V = ((O[J] & 224) >> 5);
        TimerDown.MaxTones = V;
        TimerDown.MaxHours = clamp((O[J] & 31),0,23);
    }
    J++; if (L > J) TimerDown.MaxMins = clamp(O[J],0,59);

    for (K = 0; K < 4; K++){
        J++; if (L > J){
            V = ((O[J] & 224) >> 5);
            Alarms_Repeats[K] = V;
            Alarms_Hour[K] = clamp((O[J] & 31),0,23);
        }
        J++; if (L > J) Alarms_Minutes[K] = clamp(O[J],0,59);
        J++; if (L > J + 1){
            V = ((O[J + 1] & 255) << 8);
            Alarms_Active[K] = ((O[J] | V) & ALARM_NOTRIGGER); J++;
        }
    }

    S = ""; for (I = 0; I < 10; I++) { strcpy(GSRWiFi.AP[I].APID,S.c_str()); strcpy(GSRWiFi.AP[I].PASS,S.c_str()); }

    J++; A = 0;
    while (L > J){
        Ok = false;
        if (L > J){ // get APx
            W = O[J]; J++; S = "";
            if (L > J + (W - 1)) {  // Read APx.
                for (I = 0; L > J && I < W; I++){
                     S += String(char(O[J])); J++;
                }
                strcpy(GSRWiFi.AP[A].APID,S.c_str());
                Ok = true;
            }
            if (L > J){ // get APx
                W = O[J]; J++; S = "";
                if (L > J + (W - 1)) {  // Read PAx.
                    for (I = 0; L > J && I < W; I++){
                        S += String(char(O[J])); J++;
                    }
                    strcpy(GSRWiFi.AP[A].PASS,S.c_str());
                    Ok = true;
                }
            }
        }
        if (Ok) A++;
    }

    OTAFail = millis() - 598000;
}

// Turbo Mode!
void WatchyGSR::SetTurbo(){
    Turbo.On=true;
    Turbo.Last=millis();
    LastButton=millis();  // Here for speed.
}

bool WatchyGSR::InTurbo() { return (Turbo.On && millis() - Turbo.Last < (Options.Turbo * 1000)); }

// Debugging here.

void WatchyGSR::DBug(String Value){
    if (USEDEBUG){
        if (!SerialSetup){
            Serial.begin(115200);
            SerialSetup=true;
        }
        Serial.println(Value);
    }
}

String WatchyGSR::ToHex(uint64_t Value){
  unsigned long long1 = (unsigned long)((Value & 0xFFFF0000) >> 16 );
  unsigned long long2 = (unsigned long)((Value & 0x0000FFFF));
  return String(long1, HEX) + String(long2, HEX);
}
