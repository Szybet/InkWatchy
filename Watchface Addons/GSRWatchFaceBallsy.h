// Place all of your data and variables here.

//RTC_DATA_ATTR uint8_t MyStyle;  // Remember RTC_DATA_ATTR for your variables so they don't get wiped on deep sleep.
RTC_DATA_ATTR int MyBallsyFace;	// Watchface ID #.
int MyBallsyFaceState = 0;
unsigned long MyBallsyFacePaws;

class GSRWatchFaceBallsy : public WatchyGSR {
/*
 * Keep your functions inside the class, but at the bottom to avoid confusion.
 * Be sure to visit https://github.com/GuruSR/Watchy_GSR/blob/main/Override%20Information.md for full information on how to override
 * including functions that are available to your override to enhance functionality.
*/
    public:
    GSRWatchFaceBallsy() : WatchyGSR() { initAddOn(this); } // *** DO NOT EDIT THE CONTENTS OF THIS FUNCTION ***

/*
    void InsertPost(){
    };
*/

/*
    String InsertNTPServer() {
    };
*/

/*
    void InsertDefaults(){
    };
*/

/*
    bool OverrideBitmap(){
      return false;
    };
*/

/*
    void InsertOnMinute(){
    };
*/

/*
    void InsertWiFi(){
       switch (MyFaceState){
         case 0:
           if (!Geo.beginGeoFromWeb()) MyFaceState = 2;
           MyFacePaws = millis() + 2000;  // 2 second wait.
           MyFaceState++;
           break;
         case 1:
           if (Geo.gotGeoFromWeb()) MyFaceState = 3;
           else if (millis() > MyFacePaws) MyFaceState++;
           break;
         case 2:
           Geo.endGeoFromWeb();
           MyFaceState = 0;
           endWiFi();
           break;
         case 3:
           Geo.endGeoFromWeb();
           display.setCursor(0,32);
           display.println(Geo.City);
           display.println(Geo.Longitude);
           display.println(Geo.Latitude);
           display.display(true);
           // Do the next stuff for your app here.
           MyFaceState = 0;
           endWiFi();
       }
    };
*/

/*
    void InsertWiFiEnding(){
    };
*/

// The next 3 functions allow you to add your own WatchFaces, there are examples that do work below.

    void InsertInitWatchStyle(uint8_t StyleID){
      if (StyleID == MyBallsyFace){ // Ballsy
          Design.Menu.Top = 117;
          Design.Menu.Header = 25;
          Design.Menu.Data = 66;
          Design.Menu.Gutter = 3;
          Design.Menu.Font = &aAntiCorona12pt7b;
          Design.Menu.FontSmall = &aAntiCorona11pt7b;
          Design.Menu.FontSmaller = &aAntiCorona10pt7b;
          Design.Face.Bitmap = nullptr;
          Design.Face.SleepBitmap = nullptr;
          Design.Face.Gutter = 4;
          Design.Face.Time = 56;
          Design.Face.TimeHeight = 45;
          Design.Face.TimeColor = GSR_AutoFore;
          Design.Face.TimeFont = &aAntiCorona36pt7b;
          Design.Face.TimeLeft = 0;
          Design.Face.TimeStyle = WatchyGSR::dCENTER;
          Design.Face.Day = 54;
          Design.Face.DayGutter = 4;
          Design.Face.DayColor = GSR_AutoFore;
          Design.Face.DayFont = &aAntiCorona16pt7b;
          Design.Face.DayFontSmall = &aAntiCorona15pt7b;
          Design.Face.DayFontSmaller = &aAntiCorona14pt7b;
          Design.Face.DayLeft = 0;
          Design.Face.DayStyle = WatchyGSR::dCENTER;
          Design.Face.Date = 106;
          Design.Face.DateGutter = 4;
          Design.Face.DateColor = GSR_AutoFore;
          Design.Face.DateFont = &aAntiCorona15pt7b;
          Design.Face.DateFontSmall = &aAntiCorona14pt7b;
          Design.Face.DateFontSmaller = &aAntiCorona13pt7b;
          Design.Face.DateLeft = 0;
          Design.Face.DateStyle = WatchyGSR::dCENTER;
          Design.Face.Year = 160;
          Design.Face.YearLeft = 99;
          Design.Face.YearColor = GSR_AutoFore;
          Design.Face.YearFont = &aAntiCorona14pt7b;
          Design.Face.YearLeft = 0;
          Design.Face.YearStyle = WatchyGSR::dCENTER;
          Design.Status.WIFIx = 40;
          Design.Status.WIFIy = 82;
          Design.Status.BATTx = 120;
          Design.Status.BATTy = 66;
      }
    };

    void InsertDrawWatchStyle(uint8_t StyleID){
      uint8_t X, Y;
      uint16_t A;
      if (StyleID == MyBallsyFace){
            drawDay();
            drawDate();
            if (SafeToDraw()){
                for (A = 0; A < 60; A++){
                    getAngle(A * 6, 190, 190, X, Y);
                    display.fillCircle(X + 5, Y + 5, (A == WatchTime.Local.Minute ? 5 : (A % 5 == 0 ? 3 : 1)), ForeColor());
                }
                X = WatchTime.Local.Hour;
                if (X > 11) X -= 12;
                A = (X * 30) + (WatchTime.Local.Minute / 2);
                getAngle(A, 158, 158, X, Y);
                display.fillCircle(X + 21, Y + 21, 9, ForeColor());
                if (WatchTime.Local.Hour < 12) display.fillCircle(X + 21, Y + 21, 3, BackColor());
            }
            if (NoMenu()) drawYear();
            drawWeather();
      }
    };

// Below is the Weather drawing called by drawWeather, do your weather drawing work here.
/*
    void InsertDrawWeather(uint8_t StyleID){
    };
*/

/*
    bool InsertHandlePressed(uint8_t SwitchNumber, bool &Haptic, bool &Refresh) {
      switch (SwitchNumber){
        case 2: //Back
          return true;  // Respond with "I used a button", so the WatchyGSR knows you actually did something with a button.
          break;
        case 3: //Up
          MyFaceState = 0;
          AskForWiFi();
          Haptic = true;  // Cause Haptic feedback if set to true.
          Refresh = true; // Cause the screen to be refreshed (redrawn).
          return true;
          break;
        case 4: //Down
          display.setCursor(0,16);
          display.print("Down");
          display.display(true);
          return true;
      }
      return false;
    };
*/

/*
    bool OverrideSleepBitmap(){
   };
*/

    void RegisterWatchFaces(){	// Add WatchStyles here (this is done pre-boot, post init)
        MyBallsyFace = AddWatchStyle("Ballsy",this);
   };
};

GSRWatchFaceBallsy MyBallsy;
