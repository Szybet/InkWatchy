#include <core_version.h>
#include "Defines_GSR.h"
#include "Web-HTML.h"
#include <Arduino_JSON.h>
#include <esp_partition.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <SmallRTC.h>
#include <SmallNTP.h>
#include <Olson2POSIX.h>
#include <GxEPD2_BW.h>
#include "Locale_GSR.h"
#include <mbedtls/base64.h>
#include <Wire.h>
#include <StableBMA.h>
#include "Fonts_GSR.h"
#include "Icons_GSR.h"
#include "ArduinoNvs.h"
#include <esp32-hal.h>

RTC_DATA_ATTR SmallRTC WatchyGSR::SRTC;
RTC_DATA_ATTR SmallNTP WatchyGSR::SNTP;

void WatchyGSR::init(String datetime) {
    uint64_t wakeupBit;
    bool DoOnce, B, Up;
    unsigned long Since, APLoop;
    uint8_t I;
    String S;
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
    wakeupBit = esp_sleep_get_ext1_wakeup_status();
    Wire.begin(SDA, SCL); //init i2c
    NVS.begin();

    DoOnce = true;
    IDidIt = false;
    OTAUpdate = false;
    OTAEnd = false;
    WatchyAPOn = false;
    AlarmReset = false;
    AlarmsOn = false;
    Sensitive = false;
    Updates.Drawn = false; // Force it here.
    Updates.Init = true;
    Updates.Tapped = false;
    LastButton = 0;
    Button = 0;
    Missed = 0;
    OTATry = 0;
    Darkness.Last = 0;
    Darkness.Tilt = 0;
    Darkness.Woke = false;
    TurboTime = 0;
    AllowHaptic = true;
    SoundHandle = NULL;
    GSRHandle = NULL;
    CPUSet.Freq=getCpuFrequencyMhz();

//    for (I = 0; I < 40; I++) {
//      if ((PinModeIgnore >> I) & 0b1)
//        continue;
//      pinMode(I, INPUT);
//    }

    pinMode(GSR_MENU_PIN, INPUT);   // Prep these for the loop below.
    pinMode(GSR_BACK_PIN, INPUT);
    pinMode(GSR_UP_PIN, INPUT);
    pinMode(GSR_DOWN_PIN, INPUT);


    switch (wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
            RefreshCPU(GSR_CPUDEF);
            SRTC.clearAlarm();
            UpdateUTC();
            ManageDrift();
            IDidIt = true;
            WatchTime.NewMinute = true;
            UpdateClock();
            detectBattery();
            CalculateTones();
            UpdateDisp=Showing() | (GuiMode == GSR_MENUON);
            break;
        case ESP_SLEEP_WAKEUP_EXT1: //button Press
            RefreshCPU(GSR_CPUDEF);
//            SRTC.clearAlarm();
            UpdateUTC();
            Button = getButtonMaskToID(wakeupBit);
            B = (Options.SleepStyle != 4);
            if (B) UpdateDisp |= Showing();
            if (Darkness.Went && UpRight()){
                if (Button == 9 && Options.SleepStyle > 1 && B){  // Accelerometer caused this.
                    if (Options.SleepMode == 0) Options.SleepMode = 2;  // Do this to avoid someone accidentally not setting this before usage.
                    DisplayWake(true); // Update Screen to new state.
                }else if (Button == 10 && !WatchTime.BedTime){  // Wrist.
                    DisplayWake(); // Do this anyways, always.
                }else if (Button) {
                    DisplayWake(); LastButton = millis(); Button = 0; // Update Screen to new state. if (Options.SleepStyle > 1 && B) 
                }
            }
            if (Darkness.Woke || Button) UpdateClock();  // Make sure this is done when buttons are pressed for a wakeup.
            break;
        default: //reset
            MonitorTo = this;
            WatchStyles.Count = 0;
            BasicWatchStyles = -1;
            SRTC.init();
            Battery.MinLevel = SRTC.getRTCBattery();
            Battery.LowLevel = SRTC.getRTCBattery(true);
            GSR_UP_PIN = 32;
            GSR_UP_MASK = GPIO_SEL_32;
            HWVer = SRTC.getWatchyHWVer();
            if (SRTC.getType() == PCF8563){ if (HWVer == 1.5) { GSR_UP_PIN = 32; GSR_UP_MASK = GPIO_SEL_32; } else { GSR_UP_PIN = 35; GSR_UP_MASK = GPIO_SEL_35; } }
            BTN_MASK = GSR_MENU_MASK|GSR_BACK_MASK|GSR_UP_MASK|GSR_DOWN_MASK;
//            initZeros();
            setupDefaults();
            _bmaConfig();
            Rebooted=true;
            Battery.Last = getBatteryVoltage();
            Battery.LastTest = 0;
            Battery.State = 0;
            Battery.DarkState = 0;
            Battery.Direction = -1;
            Battery.DarkDirection = 0;
            if (DefaultWatchStyles){
                I = AddWatchStyle("{%0%}"); // Classic GSR
                BasicWatchStyles = I;
            }
            InsertAddWatchStyles();
            if (WatchStyles.Count == 0){
                I = AddWatchStyle("{%0%}"); // Classic GSR
                BasicWatchStyles = I;
                DefaultWatchStyles = true;
            }
            for (I = 0; I < BootAddOns.Count; I++) { if (BootAddOns.Inits[I] != nullptr) BootAddOns.Inits[I]->RegisterWatchFaces(); }
            UpdateUTC();
            if (OkNVS(GName)) B = NVS.getString(GTZ,S);
            OP.setCurrentPOSIX(S);
            RetrieveSettings();
            if (OkNVS(GName)) {
                S=""; B = NVS.getString(STP,S); if (S.toInt()) Steps.Cached = S.toInt();
                S=""; B = NVS.getString(YSTP,S); if (S.toInt()) Steps.Yesterday = S.toInt();
                NVS.erase(STP);
                NVS.erase(YSTP);
                NVS.commit();
            }
            RefreshCPU(GSR_CPUDEF);
            WatchTime.WatchyRTC = esp_timer_get_time() + ((60 - WatchTime.UTC.Second) * 1000000);
            WatchTime.EPSMS = (millis() + (1000 * (60 - WatchTime.UTC.Second)));
            UpdateClock();
            InsertPost();
            Updates.Full=true;
#ifndef GxEPD2DarkBorder
            Options.Border=false;
#endif
            if (Options.GameCount > 0){
                if (Options.Game == 255) B = ChangeGame();
                if (Options.Game != 255) initGame(Options.Game);
            }
            WatchFaceStart(Options.WatchFaceStyle);
            StartNTP(true);
            DisplayWake(true);
            DoHaptic = true;
            break;
    }

    B = true;
    if (Darkness.Went && GuiMode != GSR_MENUON)
    {
        B = (Battery.DarkState != Battery.State || Battery.DarkDirection != Battery.Direction);
        if (Options.SleepStyle == 4) B |= Updates.Tapped; else B |= (Darkness.Woke || Button);
        if (!B && Options.Game != 255 && WatchTime.NewMinute) { if (WatchStyles.AddOn[Options.Game] != nullptr) WatchStyles.AddOn[Options.Game]->InsertOnMinute(); }
    }

    if (B || Updates.Full || WatchTime.NewMinute){

        // Sometimes BMA crashes - simply try to reinitialize bma...

        if (SBMA.getErrorCode() != 0) {
            SBMA.shutDown();
            SBMA.wakeUp();
            SBMA.softReset();
            _bmaConfig();
        }

        ActiveMode = true;
        RefreshCPU((Darkness.Went && GuiMode != GSR_MENUON && WatchTime.BedTime) ? GSR_CPULOW : 0); // Use low CPU usage at night.

        while(ActiveMode || UpdateDisp){
              Since=millis();
              ManageTime();   // Handle Time method.
              Up=SBMA.IsUp(); //SBMA.IsUp();
              B = DarkWait();
/* Pre-Tilt */

              if (!B && !Up) GoDark();

/* Tilt */
              // Wrist Tilt delay, keep screen on during this until you put your wrist down.
              if ((Options.SleepStyle == 1 || (Options.SleepStyle > 2 && Options.SleepStyle != 4) || WatchTime.DeadRTC) && !WatchTime.BedTime){
                if (Darkness.Went && Up && !Darkness.Woke){ // Do this when the wrist is UP.
                  if (Darkness.Tilt == 0) Darkness.Tilt = millis();
                  else if (millis() - Darkness.Tilt > 999) { Darkness.Last = millis(); Darkness.Woke = true; UpdateDisp |= Showing(); }
                }
                if (!Up) Darkness.Tilt = 0;
                else { if (B) Darkness.Last = millis(); if (Darkness.Tilt != 0 && millis() - Darkness.Tilt > 999 && Darkness.Woke) { Darkness.Last = millis(); } }
              }

              // Here, check for button presses and respond, done here to avoid turbo button presses.

              if (Button) { handleButtonPress(Button); Button = 0; }
/* WIFI */
              processWiFiRequest(); // Process any WiFi requests.

/* ALARMS */
              CalculateTones();

/* Insert */
              if (currentWiFi() == WL_CONNECTED && NTPData.State == 0 && WeatherData.State == 0 && !OTAUpdate && !WatchyAPOn) { if (WatchStyles.AddOn[Options.WatchFaceStyle] == nullptr) InsertWiFi(); else WatchStyles.AddOn[Options.WatchFaceStyle]->InsertWiFi(); }

/* NTP */

              if (Battery.Last > Battery.MinLevel && WatchTime.UTC_RAW >= WeatherData.LastCall) StartWeather();
              if (NTPData.State && WeatherData.State < 2 && !WatchyAPOn && !OTAUpdate){
                if (NTPData.Pause == 0) ProcessNTP(); else NTPData.Pause--;
                if (WatchTime.NewMinute){
                  NTPData.Wait++;
                  UpdateDisp |= Showing();
                }
              }

/* Weather */
              if (WeatherData.State && NTPData.State < 2 && !WatchyAPOn && !OTAUpdate){

                if (WeatherData.Pause == 0) ProcessWeather(); else WeatherData.Pause--;
                if (WatchTime.NewMinute){
                  WeatherData.Wait++;
                  UpdateDisp |= Showing();
                }
              }

              if (GuiMode == GSR_WATCHON && GetMenuOverride()) {
                if (Missed == 0) Missed = getButtonPins();
                if ((Missed != 1 && Button) || (Missed == 1 && LastHelp == 0)) { LastHelp = millis(); SetTurbo(); }
                else if (Missed == 1 && millis() - LastHelp > 9999) { ShowDefaultMenu(); Missed = 0; }
              }
              if (Missed){
                if (GuiMode == GSR_WATCHON) { if (WatchStyles.AddOn[Options.WatchFaceStyle] == nullptr) { if (InsertHandlePressed(Missed, DoHaptic, UpdateDisp)) SetTurbo(); } else if (WatchStyles.AddOn[Options.WatchFaceStyle]->InsertHandlePressed(Missed, DoHaptic, UpdateDisp)) SetTurbo(); }
                else if (GuiMode == GSR_GAMEON && Options.Game != 255) { if (WatchStyles.AddOn[Options.Game]->InsertHandlePressed(Missed, DoHaptic, UpdateDisp)) SetTurbo(); }
                Missed = 0; Button = 0;
              }

              if (UpdateDisp) showWatchFace(); //partial updates on tick

/* SOUNDS */
              if (SoundStart) SoundBegin();

/* OTA */
              // OTAEnd code.
              if (OTAEnd){
                if (Menu.Item == GSR_MENU_OTAU)      ArduinoOTA.end();
                else if (Menu.Item == GSR_MENU_OTAM) server.stop();
                if (WatchyAPOn) server.stop();
                VibeTo(false);
                OTAEnd=false;
                endWiFi();
                if (OTAUpdate) Menu.SubItem=0;
                UpdateUTC();
                UpdateClock();
                OTAUpdate=false;
                WatchyAPOn = false;
                Battery.UpCount=0;  // Stop it from thinking the battery went wild.
                Missed = 0; Button = 0;
                UpdateDisp |= Showing();
              }

/* OTA */
              if (OTAUpdate){
                if (!GetAskWiFi()) AskForWiFi();
                else {
                  switch (Menu.SubItem){
                    case 1: // Wait for WiFi to connect or fail.
                      if (WiFiStatus() != WL_CONNECTED && currentWiFi() != WL_CONNECT_FAILED) OTATimer = millis();
                      else if (WiFiStatus() == WL_CONNECTED){
                        Menu.SubItem++;
                        UpdateDisp |= Showing();
                      }else OTAEnd=true;
                      break;
                    case 2: // Setup Arduino OTA and wait for it to either finish or fail by way of back button held for too long OR 2 minute with no upload.
                      if (Menu.Item == GSR_MENU_OTAU){
                      ArduinoOTA.setHostname(WiFi_AP_SSID);
                      ArduinoOTA
                      .onStart([]() {
                        String Type;
                        if (ArduinoOTA.getCommand() == U_FLASH)
                        Type = "sketch";
                        else // U_SPIFFS
                        Type = "filesystem";
                        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                      })
                      .onEnd([]() {
                        OTAEnd = true;
                      })
                      .onProgress([](unsigned int progress, unsigned int total) {
                        OTATimer=millis();
                      })
                      .onError([](ota_error_t error) {
                        OTAEnd=true;
                      });
                      RefreshCPU(GSR_CPUMAX);
                      ArduinoOTA.begin();
                      }else if (Menu.Item == GSR_MENU_OTAM) WatchyGSR::StartWeb();
                      Menu.SubItem++;
                      showWatchFace();
                      break;
                    case 3: // Monitor back button and turn WiFi off if it happens, or if duration is longer than 2 minutes.
                      if (WiFiStatus() == WL_DISCONNECTED) OTAEnd = true;
                      else if (Menu.Item == GSR_MENU_OTAU)      ArduinoOTA.handle();
                      else if (Menu.Item == GSR_MENU_OTAM)      server.handleClient();
                      if (getButtonPins() != 2) OTATimer = millis(); // Not pressing "BACK".
                      if (millis() - OTATimer > 10000 || millis() - OTAFail > 600000) OTAEnd = true;  // Fail if holding back for 10 seconds OR 600 seconds has passed.
                  }
                }
              }

/* NON-SENSITIVE */
              // Don't do anything time sensitive while in OTA Update.
              if (!Sensitive){

/* Battery Detection */

                detectBattery();

                // Here, make sure the clock updates on-screen.
                if (WatchTime.NewMinute) UpdateDisp |= Showing();

/* WATCHY AP */

              if (GSRWiFi.Requests == 0 && WatchyAPOn && !OTAUpdate){
                switch (Menu.SubItem){
                  case 0: // Turn off AP.
                    OTAEnd = true;
                    break;
                  case 1: // Turn on AP.
                    if (WiFi.getMode() != WIFI_AP || (millis() - OTATimer > 4000 && OTATry < 3)){
                      OTATimer=millis();
                      OTATry++;
                      WiFi.setHostname(WiFi_AP_SSID);
                      OTAEnd |= (!WiFi.softAP(WiFi_AP_SSID, WiFi_AP_PSWD, 1, WiFi_AP_HIDE, WiFi_AP_MAXC));
                      if (!OTAEnd) UpdateWiFiPower();
                    }else if (WiFi.getMode() == WIFI_AP){
                      WatchyGSR::StartWeb();
                      Menu.SubItem++;
                      setStatus("WiFi-AP");
                      APLoop=millis();
                    }else Menu.SubItem = 0; // Fail, something is amiss.
                    break;
                  default: // 2 to 5 is here.
                    if (Menu.SubItem > 1){
                      if(WiFi.getMode() == WIFI_STA){
                        Menu.SubItem = 0;
                        break;
                      }
                      server.handleClient();
                      /*if (Server.handleRequests()){
                        Menu.SubItem = 0;
                        break;
                      }*/
                      if (millis() - APLoop > 8000){
                        Menu.SubItem = roller(Menu.SubItem + 1, 2,4);
                        UpdateDisp |= Showing();
                        APLoop=millis();
                      }
                    }
                }
              }

              if (WatchTime.DeadRTC && Options.NeedsSaving) RecordSettings();
              CheckButtons();

              if (!Updates.Init && !SoundActive() && !(InTurbo() || B)) DisplaySleep();
            }
            Sensitive = (OTAUpdate && Menu.SubItem == 3);
            AlarmsOn =(Alarms_Times[0] || Alarms_Times[1] || Alarms_Times[2] || Alarms_Times[3] || TimerDown.ToneLeft || SoundActive() || !GetWebAvailable() || TimerAbuse());
            ActiveMode = (InTurbo() || B || NTPData.State || WeatherData.State || AlarmsOn || WatchyAPOn || OTAUpdate || WatchTime.DeadRTC || GSRWiFi.Requested || GSRWiFi.Requests || OTAEnd || Button); /* (B && Up) */
            if (Options.Game != 255) { if (WatchStyles.AddOn[Options.Game] != nullptr) ActiveMode |= WatchStyles.AddOn[Options.Game]->InsertNeedAwake(!ActiveMode); }
            if (WatchStyles.AddOn[Options.WatchFaceStyle] == nullptr) ActiveMode |= InsertNeedAwake(!ActiveMode); else ActiveMode |= WatchStyles.AddOn[Options.WatchFaceStyle]->InsertNeedAwake(!ActiveMode);

            WatchTime.NewMinute=false;
            IDidIt=false;

            RefreshCPU(GSR_CPUDEF);
            Since=50-(millis()-Since);
            if (Since <= 50 && Since) delay(Since);
        }
    }
    deepSleep();
}


void WatchyGSR::showWatchFace(){
  bool B = (Battery.Last > Battery.MinLevel);
  if (Options.Performance && B) if (Options.Performance == 1) RefreshCPU(GSR_CPUMID); else if (Options.Performance == 2) RefreshCPU(GSR_CPULOW);
  if (Options.Feedback && DoHaptic && B && AllowHaptic) { HapticMS = 10; SoundBegin(); }
  DisplayInit();
  display.setFullWindow();
  if (GuiMode == GSR_GAMEON) drawGame(); else drawWatchFace();
  //RefreshCPU(GSR_CPULOW);
  CheckButtons();
  display.display(!Updates.Full); //partial refresh
  if (!(InTurbo() || SoundActive() || DarkWait())) DisplaySleep();
  DoHaptic=false;
  Updates.Full=false;
  Updates.Drawn=true;
  //if (B) RefreshCPU(GSR_CPUDEF);
  UpdateDisp=false;
  Darkness.Went=false;
  Darkness.Last = millis();
}

void WatchyGSR::drawWatchFace(){
    display.fillScreen(BackColor());
    display.setTextWrap(false);
    if (WatchStyles.AddOn[Options.WatchFaceStyle] == nullptr) { if (!OverrideBitmap()) { if (Design.Face.Bitmap) display.drawBitmap(0, 0, Design.Face.Bitmap, 200, 200, ForeColor(), BackColor()); } } else { if (!WatchStyles.AddOn[Options.WatchFaceStyle]->OverrideBitmap()) { if (Design.Face.Bitmap) display.drawBitmap(0, 0, Design.Face.Bitmap, 200, 200, ForeColor(), BackColor()); } }
    display.setTextColor(ForeColor());

    drawWatchFaceStyle();

    if (!GetNoStatus()){
        drawChargeMe();
        // Show WiFi/AP/TZ/NTP if in progress.
        drawStatus();
    }
    if (GuiMode == GSR_MENUON) drawMenu();
}

void WatchyGSR::drawDay(){
    String O = LGSR.GetFormatID(Options.LanguageID,0);
    O.replace("{W}",LGSR.GetWeekday(Options.LanguageID, WatchTime.Local.Wday));
    setFontFor(O,Design.Face.DayFont,Design.Face.DayFontSmall,Design.Face.DayFontSmaller,Design.Face.DayGutter);
    setFontColor(Design.Face.DayColor);
    drawData(O, Design.Face.DayLeft, Design.Face.Day, Design.Face.DayStyle, Design.Face.DayGutter);
}

void WatchyGSR::drawDate(bool Short){
    String O = LGSR.GetFormatID(Options.LanguageID,1);
    O.replace("{M}",(Short ? LGSR.GetShortMonth(Options.LanguageID, WatchTime.Local.Month) : LGSR.GetMonth(Options.LanguageID, WatchTime.Local.Month)));
    O.replace("{D}",String(WatchTime.Local.Day));
    setFontFor(O,Design.Face.DateFont,Design.Face.DateFontSmall,Design.Face.DateFontSmaller,Design.Face.DateGutter);
    setFontColor(Design.Face.DateColor);
    drawData(O, Design.Face.DateLeft, Design.Face.Date, Design.Face.DateStyle, Design.Face.DateGutter);
}

void WatchyGSR::drawYear(){
    display.setFont(Design.Face.YearFont);
    setFontColor(Design.Face.YearColor);
    drawData(String(WatchTime.Local.Year + SRTC.getLocalYearOffset()), Design.Face.YearLeft, Design.Face.Year, Design.Face.YearStyle, Design.Face.Gutter);  //1900
}

void WatchyGSR::drawData(String dData, byte Left, byte Bottom, WatchyGSR::DesOps Style, byte Gutter, bool isTime, bool PM){
    uint16_t w, Width, Height, Ind;
    int16_t X, Y;

    display.getTextBounds(dData, Left, Bottom, &X, &Y, &Width, &Height);

    Bottom = constrain(Bottom, Gutter, 200 - Gutter);
    switch (Style){
        case WatchyGSR::dLEFT:
            Left = Gutter;
            break;
        case WatchyGSR::dRIGHT:
            Left = constrain(200 - (Gutter + Width), Gutter, 200 - Gutter);
            break;
        case WatchyGSR::dSTATIC:
            Left = constrain(Left, Gutter, 200 - Gutter);
            break;
        case WatchyGSR::dCENTER:
            Left = constrain(4 + ((196 - (Gutter + Width)) / 2), Gutter, 200 - Gutter);
            break;
    };
    display.setCursor(Left, Bottom);
    display.print(dData);

    if (isTime && PM){
        if (Style == WatchyGSR::dRIGHT) Left = constrain(Left - 12, Gutter, 200 - Gutter);
        else Left = constrain(Left + Width + 6, Gutter, 190);
        display.drawBitmap(Left, Bottom - Design.Face.TimeHeight, PMIndicator, 6, 6, ForeColor());
    }
}

void WatchyGSR::setFontFor(String O, const GFXfont *Normal, const GFXfont *Small, const GFXfont *Smaller, byte Gutter){
    int16_t  x1, y1;
    uint16_t w, h;
    display.setTextWrap(false);
    byte wi = (200 - (2 * Gutter));
    display.setFont(Normal); display.getTextBounds(O, 0, 0, &x1, &y1, &w, &h);
    if (w > wi) { display.setFont(Small); display.getTextBounds(O, 0, 0, &x1, &y1, &w, &h); }
    if (w > wi) { display.setFont(Smaller); display.getTextBounds(O, 0, 0, &x1, &y1, &w, &h); }
}


void WatchyGSR::deepSleep(){
  uint8_t N, D, H; // 3.0
  bool BatOk, BT,B, DM, CD, M;
  struct tm * CT;
  DM = false; CD=TimerDown.Active;
  UpdateUTC(); UpdateClock(); M =(GuiMode != GSR_MENUON);
  if (CD && (TimerDown.StopAt - WatchTime.UTC_RAW) > 60) CD = false;

  B = false; VibeTo(false);
  UpdateBMA();
  DM = (Darkness.Went && !CD && M);

  if (DM){
    H = WatchTime.Local.Hour;
    BatOk = (Battery.Last == 0 || Battery.Last > Battery.LowLevel);
    BT = (Options.SleepStyle == 2 && WatchTime.BedTime);
    B = ((Options.SleepStyle == 1 || (Options.SleepStyle > 2 && Options.SleepStyle != 4)) || BT);
    if (Battery.Direction == 1 || Battery.State == 3) N = (WatchTime.UTC.Minute - (WatchTime.UTC.Minute % 5) + 5) % 60; else N = (WatchTime.UTC.Minute < 30 ? 30 : 60);
    if (WatchTime.NextAlarm != 99){ if (Alarms_Minutes[WatchTime.NextAlarm] > WatchTime.Local.Minute && Alarms_Minutes[WatchTime.NextAlarm] < N) N = Alarms_Minutes[WatchTime.NextAlarm]; }
    if (TimerDown.Active){
      CT=localtime(&TimerDown.StopAt);
      if (CT->tm_hour == H && CT->tm_min < N && CT->tm_min > WatchTime.Local.Minute) N = CT->tm_min;
    }
    if (!WatchTime.DeadRTC && String(WeatherData.APIKey) != "" && WeatherData.LastCall != 0 && GetWantWeather()){
      CT=localtime(&WeatherData.LastCall);
      if (CT->tm_hour == H && CT->tm_min < N && CT->tm_min > WatchTime.Local.Minute) N = CT->tm_min;
    }
    if (Steps.Hour == H && Steps.Minutes < N && Steps.Minutes > WatchTime.Local.Minute) N = Steps.Minutes;
    if (NTPData.TravelTest == 0 && NTPData.AutoSync && NTPData.SyncHour == H && NTPData.SyncMins < N && NTPData.SyncMins > WatchTime.Local.Minute) N = NTPData.SyncMins;
  }

  if (Options.NeedsSaving) RecordSettings();
  GoDark(M); DisplaySleep();
  if (DM) SRTC.atMinuteWake(N); else SRTC.nextMinuteWake();
  ForceInputs();
  esp_sleep_enable_ext0_wakeup((gpio_num_t)GSR_RTC_INT_PIN, 0); //enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext1_wakeup((B ? SBMA.WakeMask() : 0) | BTN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press  ... |ACC_INT_MASK
  esp_deep_sleep_start();
}


void WatchyGSR::GoDark(bool DeepSleeping){
  bool B;
  if (Options.SleepStyle == 0 || (Options.SleepStyle == 2 && !WatchTime.BedTime) || GuiMode == GSR_MENUON) return;
  if ((Updates.Drawn || !Darkness.Went) && (DeepSleeping || !Showing()))
  {
    Darkness.Went=true;
    Darkness.Woke=false;
    Darkness.Tilt=0;
    Updates.Init=Updates.Drawn;
    display.setFullWindow();
    DisplayInit(true);  // Force it here so it fixes the border.
    display.fillScreen(GxEPD_BLACK);
    if (WatchStyles.AddOn[Options.WatchFaceStyle] == nullptr) { if (!OverrideSleepBitmap()) { if (Design.Face.SleepBitmap) display.drawBitmap(0, 0, Design.Face.SleepBitmap, 200, 200, GxEPD_WHITE, GxEPD_BLACK); } } else { if (!WatchStyles.AddOn[Options.WatchFaceStyle]->OverrideSleepBitmap()) { if (Design.Face.SleepBitmap) display.drawBitmap(0, 0, Design.Face.SleepBitmap, 200, 200, GxEPD_WHITE, GxEPD_BLACK); else B = OverrideSleepBitmap(); } }
    drawChargeMe(true);
    drawStatus(true);
    Battery.DarkDirection = Battery.Direction;
    Battery.DarkState = Battery.State;
    display.display(true);
    Updates.Drawn=false;
    display.hibernate();
    LastHelp = 0;
  }
}


void WatchyGSR::ForceInputs(){
uint8_t P = SRTC.getADCPin();
/* Unused GPIO PINS */
    pinMode(0,INPUT);   /*  ??      */
    pinMode(2,INPUT);   /*  ??      */
    pinMode(13,INPUT);  /*  ??      */
    pinMode(15,INPUT);  /*  ??      */
    pinMode(20,INPUT);  /*  ??      */
    pinMode(36,INPUT);  /*  ??      */
    pinMode(37,INPUT);  /*  ??      */
    pinMode(38,INPUT);  /*  ??      */
    pinMode(39,INPUT);  /*  ??      */
/* RS232 */
    pinMode(3,INPUT);   /*  RX      */
/* BUTTONS */
    pinMode(26,INPUT);  /* BTN1     */
    pinMode(25,INPUT);  /* BTN2     */
    if (GSR_UP_PIN != 32) pinMode(32,INPUT);  /* BTN3     */
    if (!(GSR_UP_PIN == 35 || P == 35)) pinMode(35,INPUT);  /* ADC/BTN3 */
    pinMode(4,INPUT);   /* BTN4     */
/* BATTERY */
    //pinMode(27,INPUT);  /* INT      */
    //if (P != 33) pinMode(33,INPUT);  /* ADC      */
    //if (P != 34) pinMode(34,INPUT);  /* ADC      */
/* RTC */
    pinMode(21,INPUT);  /* SDA      */
    pinMode(22,INPUT);  /* SCL      */
/* DISPLAY */
    pinMode(5,INPUT);   /*  CS      */
    pinMode(9,INPUT);   /* RES      */
    pinMode(10,INPUT);  /*  DC      */
    pinMode(18,INPUT);  /* SLCK     */
    pinMode(19,INPUT);  /* BUSY     */
    pinMode(23,INPUT);  /* MOSI     */
/* BMA 423 */
    pinMode(12,INPUT);  /* INT2     */
    pinMode(14,INPUT);  /* INT1     */
}

void WatchyGSR::detectBattery(){
    float CBAT, BATOff, Diff;
    uint8_t Mins = 0;
    if (Battery.LastTest > WatchTime.UTC_RAW) return;
    Diff = ((WatchTime.UTC_RAW - Battery.LastTest) / 60) - 1;
    if (Diff > 0) Mins = golow(Diff,3);
    Battery.LastTest = WatchTime.UTC_RAW + 30;  /* Testing */
    CBAT = getBatteryVoltage(); // Check battery against previous versions to determine which direction the battery is going.
    BATOff = CBAT - Battery.Last;
     //Detect Power direction
    if (BATOff > 0.05){
      Battery.UpCount++;
      if (Battery.UpCount + Mins > 3){
            if (Battery.Direction == 1) Battery.Last = CBAT;
            Battery.Direction = 1; Battery.UpCount = 0; Battery.DownCount = 0;
            // Check if the NTP has been done.
            if (WatchTime.UTC_RAW - NTPData.Last > 14400) StartNTP(true);
        }
    }else{
        if (BATOff < 0.00) Battery.DownCount++;
        if (Battery.DownCount + Mins > 2)
        {
            if (Battery.Direction == -1) Battery.Last = CBAT;
            Battery.Direction = -1; Battery.UpCount = 0; Battery.DownCount = 0;
        }
    }

    if (Battery.Last > Battery.MinLevel && NTPData.TravelTest == 0) { if (WatchTime.Local.Hour == NTPData.SyncHour && WatchTime.Local.Minute == NTPData.SyncMins && NTPData.AutoSync && WatchTime.UTC_RAW - NTPData.Last > 59) StartNTP(true); }

    BATOff = Battery.State; /* Set update for display if the battery state changes */

    // Do battery state here.
    if (Battery.Direction > 0) Battery.State = 3;
    else Battery.State = (Battery.Last > Battery.MinLevel ? (Battery.Last > Battery.LowLevel ? 0 : 1) : 2);
    if (BATOff != Battery.State) UpdateDisp |= Showing();
    if (!Showing() && (Battery.DarkState != Battery.State || Battery.DarkDirection != Battery.Direction)) Updates.Drawn = true; // Fix the battery indicator.
}

void WatchyGSR::drawTime(uint8_t Flags){
    String O;
    bool PM = false;
    O = MakeTime(WatchTime.Local.Hour | (Flags & 96), WatchTime.Local.Minute, PM);
    display.setFont(Design.Face.TimeFont);
    setFontColor(Design.Face.TimeColor);

    drawData(O,Design.Face.TimeLeft,Design.Face.Time,Design.Face.TimeStyle, Design.Face.Gutter, ((Flags & 32) ? false : true), PM);
}


void WatchyGSR::VibeTo(bool Mode){
    if (Mode != VibeMode){
        if (Mode){
            pinMode(GSR_VIB_MOTOR_PIN, OUTPUT);
            digitalWrite(GSR_VIB_MOTOR_PIN, true);
        }else{
            digitalWrite(GSR_VIB_MOTOR_PIN, false);
        }
        VibeMode = Mode;
    }
}

void WatchyGSR::UpdateUTC(bool OnlyRead){
    if (!WatchTime.DeadRTC){
        SRTC.read(WatchTime.UTC);
        WatchTime.UTC_RAW = SRTC.MakeTime(WatchTime.UTC);
        if (!OnlyRead) WatchTime.EPSMS = (millis() + (1000 * (60 - (WatchTime.UTC.Second))));
    }
    SRTC.BreakTime(WatchTime.UTC_RAW,WatchTime.UTC);
}

void WatchyGSR::UpdateClock(){
    WatchTime.Local = UTCtoLocal(WatchTime.UTC_RAW);
    bool B = WatchTime.BedTime;
    if (Options.SleepEnd > Options.SleepStart) WatchTime.BedTime = (WatchTime.Local.Hour >= Options.SleepStart && WatchTime.Local.Hour < Options.SleepEnd);
    else WatchTime.BedTime = (WatchTime.Local.Hour >= Options.SleepStart || WatchTime.Local.Hour < Options.SleepEnd);
/* SORT OUT THE NIGHTTIME Wallpaper Change */
    if (WatchTime.BedTime != B) { Updates.Drawn = true; } // Fake we messed up the screen.
    monitorSteps(); // Moved here for accuracy during Deep Sleep.
}

tmElements_t WatchyGSR::UTCtoLocal(time_t Incoming){
    struct tm * TM;
    tmElements_t T;

    OP.setCurrentTimeZone();
    TM = localtime(&Incoming);
    T.Second = TM->tm_sec;
    T.Minute = TM->tm_min;
    T.Hour = TM->tm_hour;
    T.Wday = TM->tm_wday;
    T.Day = TM->tm_mday;
    T.Month = TM->tm_mon;
    T.Year = TM->tm_year;
    return T;
}

void WatchyGSR::ManageDrift(){
    int S = 0;
    /*
     * Drift detection is done elsewhere, but here it accumulates a pool of MS and when it gets large enough to matter, it is added to the current time.
     */
    if (NTPData.TravelTest != 0) return;  // Avoid Travel Test.
    if (Options.Drift == 0 && Options.UsingDrift) Options.UsingDrift = false;
    if (!WatchTime.DeadRTC && NTPData.TravelTest == 0 && !TimerDown.Active && Options.UsingDrift && WatchTime.UTC_RAW > NTPData.LastDrift){
        S = (Options.FastDrift ? -1 : 1);
        NTPData.LastDrift += Options.Drift;
        WatchTime.EPSMS += (S * 1000);
        WatchTime.UTC_RAW += S;
        SRTC.BreakTime(WatchTime.UTC_RAW, WatchTime.UTC);
        SRTC.set(WatchTime.UTC);
        UpdateUTC();
        UpdateClock();
    }
}

float WatchyGSR::getBatteryVoltage(){ return ((BatteryRead() - 0.0125) +  (BatteryRead() - 0.0125) + (BatteryRead() - 0.0125) + (BatteryRead() - 0.0125)) / 4; }
float WatchyGSR::BatteryRead(){ return analogReadMilliVolts(SRTC.getADCPin()) / 500.0f; } // Battery voltage goes through a 1/2 divider.

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

void WatchyGSR::RefreshCPU(int Value){
    uint32_t C = 80;
    if (!WatchTime.DeadRTC && Battery.Last > Battery.MinLevel) {
        if (Value == GSR_CPUMAX) CPUSet.Locked = true;
        if (Value == GSR_CPUDEF) CPUSet.Locked = false;
        if (!CPUSet.Locked && Options.Performance != 2 && Value != GSR_CPULOW) C = (InTurbo() || Value == GSR_CPUMID) ? 160 : 80;
        if (WatchyAPOn || OTAUpdate || GSRWiFi.Requests > 0 || CPUSet.Locked || (Options.Performance == 0 && Value != GSR_CPULOW)) C = 240;
    }
    if (C != CPUSet.Freq) if (setCpuFrequencyMhz(C)); CPUSet.Freq = C;
}

/* This is here so that all ESP.restarts store the step counter */
void WatchyGSR::Reboot(){
    bool B;
    if (OkNVS(GName)){
        if (WatchTime.DeadRTC) RecordSettings();
        B = NVS.setString(STP,String(CurrentStepCount()));
        B = NVS.setString(YSTP,String(Steps.Yesterday));
        NVS.commit();
        NVS.close();
    }
    ESP.restart();
}

void WatchyGSR::DisplayInit(bool ForceDark){
#ifdef GxEPD2DarkBorder
  display.epd2.setDarkBorder(Options.Border | ForceDark);
#endif
  if (Updates.Init){
    display.init(0,Rebooted,10,true);  // Force it here so it fixes the border.
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    Updates.Init=false;
    Rebooted=false;
  }
}

void WatchyGSR::DisplaySleep(){ if (!Updates.Init) { Updates.Init = true; display.hibernate(); } }
