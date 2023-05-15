/* READ THIS AND GO THROUGH THE BELOW LINES:
 *
 * Using a Search and Replace to quicken configuration of this:
 * First come up with a unique name for this WatchFace AddOn:  <Put the name here>.
 * You're going to use that unique name to preface all of the below Replaces:
 * EG: MyUniqueAddOn would replace <MyWatchFace> with MyUniqueAddOn
 * Now Replace All for <MyWatchFace> with <your unique name> (as the example above).
 * Next Replace All <MyFirstWatchFace> with <your unique name for your first watchface> (as the example above).
 * Next Replace <MyFirstWatchFaceStyleName> with the name of the first WatchFace Style that would be seen in the Watchy listing.
 * Save this file as <your unique name>.h in with the GSR.ino.
 *
 * You can now compile this with Watchy_GSR, as this is a functioning AddOn (default like the base Classic GSR WatchFace Style).
 */


// Function Declarations here:  If you have any custom functions you wish to use in this file, you must declare them here:


// Place all of your data here.  All Fonts and Image Data goes below this line and before the Variables.


// Place all your Variables here.
RTC_DATA_ATTR uint8_t <MyFirstWatchFace>Style;  // Remember RTC_DATA_ATTR for your variables so they don't get wiped on deep sleep.
// If you want more than one WatchFaceStyle, make more variables and be sure to register them below in RegisterWatchFaces().

// This is used to keep track of times you're in various looping functions (like AskForWiFi calling InsertWiFi).
// Use more of these type of variables with the looping functions for various WatchFace Styles in this file.
// Looping variables don't necessarily need RTC_DATA_ATTR, unless you're using this variable during deep sleep minute wakes.
int <MyFirstWatchFace>State = 0;


class <MyWatchFace>Class : public WatchyGSR {
/*
 * Keep your functions inside the class, but at the bottom to avoid confusion.
 * Be sure to visit https://github.com/GuruSR/Watchy_GSR/blob/main/Override%20Information.md for full information on AddOns.
 * Overriding is not possible with an AddOn, you only have access to most of the Insert style functions.
 * The full available range of WatchyGSR public functions and data elements are available, you do not need to use WatchyGSR:: at the start.
 * Design elements do have a WatchyGSR:: at the start to specify those elements from WatchyGSR, if there is confusion with the compiler, use
 * WatchyGSR:: in front of items it can't distinguish or rename the ones you've added in this file to avoid the issue.
 */

    public:
    <MyWatchFace>Class() : WatchyGSR() { initAddOn(this); } // *** DO NOT EDIT THE CONTENTS OF THIS FUNCTION ***


// This function is required by AddOns and is used to register one or more WatchFace Styles at startup, you can add more here, but you need more variables above
// to track all of the WatchFaces and determine which ones match the CurrentStyleID(), as WatchFace Styles are collected and assigned at boot.

    void RegisterWatchFaces(){
      <MyFirstWatchFace>Style = AddWatchStyle("<MyFirstWatchFaceStyleName>",this);
    };


// This function sets up the WatchFace Styles, this is required for AddOns,
// as it sets up the Style when it is called for the first time and each time the Watchy is switched to another Watchface.

    void InsertInitWatchStyle(uint8_t StyleID){
      if (StyleID == <MyFirstWatchFace>Style){
          //OverrideDefaultMenu(true);
          //WantWeather(true);
          Design.Menu.Top = 72;
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
          Design.Face.Day = 101;
          Design.Face.DayGutter = 4;
          Design.Face.DayColor = GSR_AutoFore;
          Design.Face.DayFont = &aAntiCorona16pt7b;
          Design.Face.DayFontSmall = &aAntiCorona15pt7b;
          Design.Face.DayFontSmaller = &aAntiCorona14pt7b;
          Design.Face.DayLeft = 0;
          Design.Face.DayStyle = WatchyGSR::dCENTER;
          Design.Face.Date = 143;
          Design.Face.DateGutter = 4;
          Design.Face.DateColor = GSR_AutoFore;
          Design.Face.DateFont = &aAntiCorona15pt7b;
          Design.Face.DateFontSmall = &aAntiCorona14pt7b;
          Design.Face.DateFontSmaller = &aAntiCorona13pt7b;
          Design.Face.DateLeft = 0;
          Design.Face.DateStyle = WatchyGSR::dCENTER;
          Design.Face.Year = 186;
          Design.Face.YearLeft = 99;
          Design.Face.YearColor = GSR_AutoFore;
          Design.Face.YearFont = &aAntiCorona16pt7b;
          Design.Face.YearLeft = 0;
          Design.Face.YearStyle = WatchyGSR::dCENTER;
          Design.Status.Inverted = false;
          Design.Status.WIFIx = 5;
          Design.Status.WIFIy = 193;
          Design.Status.BatteryInverted = false;
          Design.Status.BATTx = 155;
          Design.Status.BATTy = 178;
      }
    };

// This function will draw the WatchFace on the screen, drawStatus and drawChargeMe functions are done elsewhere and do not need to be done here.
// This function is required for AddOns, otherwise there will be no drawing of the Watchface.
 
    void InsertDrawWatchStyle(uint8_t StyleID){
      if (StyleID == <MyFirstWatchFace>Style){
            if (SafeToDraw()){
                drawTime();
                drawDay();
                drawYear();
            }
            if (NoMenu()) drawDate();
      }
    };


/* This function can be used to return your favorite NTP server.  CurrentStyleID() can be used here to separate between Styles.

    String InsertNTPServer() { return "myfavntpserver"; }
*/


/* This function will run on each new minute.  CurrentStyleID() can be used here to separate between Styles.

    void InsertOnMinute(){
    };
*/


/* This function is called when you've asked for WiFi and no other processes are using it.  CurrentStyleID() can be used here to separate between Styles.

    void InsertWiFi(){
    };
*/

/* This function is called when WiFi is ending, clean up after your code, if it is StyleID specific, use CurrentStyleID() to read the current StyleID.

    void InsertWiFiEnding(){
    };
*/


/* This function is useful to draw the Weather on the screen, if Status is true, means the Watchy_GSR status area asked for the icons to be placed there, use the Design.Status.WIFIx and WIFIy for positioning.

void InsertDrawWeather(uint8_t StyleID, bool Status) {}
*/


/* This function is used to tell WatchyGSR that you need it to stay in Active Mode (true), so you can keep the Watchy from deep sleeping.
// CurrentStyleID() can be used here to separate between Styles.

 bool InsertNeedAwake(bool GoingAsleep) { return false; }
*/


/* This function is used when wanting buttons not being used by the Menu system and/or if you're overriding the Menu system, BUTTON MIXING works here.
// CurrentStyleID() can be used here to separate between Styles.

    bool InsertHandlePressed(uint8_t SwitchNumber, bool &Haptic, bool &Refresh) {
      switch (SwitchNumber){
        case 2: //Back
          return true;  // Respond with "I used a button", so the WatchyGSR knows you actually did something with a button.
          break;
        case 3: //Up
          State = 0;
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


/* This function can be used to override the Design's Bitmap setting.  CurrentStyleID() can be used here to separate between Styles.

    bool OverrideBitmap(){ return false; };
*/


/* This function can be used to override the Design's SleepBitmap setting.  CurrentStyleID() can be used here to separate between Styles.

    bool OverrideSleepBitmap(){ return false; }
*/

};

<MyWatchFace>Class <MyWatchFace>ClassLoader;

// Put your functions here, outside of the class.
