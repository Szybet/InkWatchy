/* READ THIS AND GO THROUGH THE BELOW LINES:
 *
 * Using a Search and Replace to quicken configuration of this:
 * First come up with a unique name for this WatchFace Game AddOn:  <Put the name here>.
 * You're going to use that unique name to preface all of the below Replaces:
 * EG: MyUniqueGameAddOn would replace <MyWatchGame> with MyUniqueGameAddOn
 * Now Replace All for <MyWatchGame> with <your unique name> (as the example above).
 * Next Replace All <MyFirstWatchGame> with <your unique name for your first watchface game> (as the example above).
 * Next Replace <MyFirstWatchGameName> with the name of the first WatchFace Game Style that would be seen in the Watchy listing.
 * Save this file as <your unique name>.h in with the GSR.ino.
 *
 * You can now compile this with Watchy_GSR, as this is a functioning Game AddOn,
 * (doesn't do much at all, BACK exits).
 */


// Function Declarations here:  If you have any custom functions you wish to use in this file, you must declare them here:


// Place all of your data here.  All Fonts and Image Data goes below this line and before the Variables.


// Place all your Variables here.
RTC_DATA_ATTR uint8_t <MyFirstWatchGame>Style;  // Remember RTC_DATA_ATTR for your variables so they don't get wiped on deep sleep.
// If you want more than one WatchFaceGameStyle, make more variables and be sure to register them below in RegisterWatchFaces().

// This is used to keep track of times you're in various looping functions.
// WiFi does not work for Game mode, so games currently cannot connect to online services through it.
// Use more of these type of variables with the looping functions for various WatchGame Styles in this file.
// Looping variables don't necessarily need RTC_DATA_ATTR, unless you're using this variable during deep sleep minute wakes.
int <MyFirstWatchGame>State = 0;


class <MyWatchGame>Class : public WatchyGSR {
/*
 * Keep your functions inside the class, but at the bottom to avoid confusion.
 * Be sure to visit https://github.com/GuruSR/Watchy_GSR/blob/main/Override%20Information.md for full information on AddOns.
 * Overriding is not possible with an AddOn, you only have access to most of the Insert style functions.
 * The full available range of WatchyGSR public functions and data elements are available, you do not need to use WatchyGSR:: at the start.
 * Design elements do have a WatchyGSR:: at the start to specify those elements from WatchyGSR, if there is confusion with the compiler, use
 * WatchyGSR:: in front of items it can't distinguish or rename the ones you've added in this file to avoid the issue.
 */

    public:
    <MyWatchGame>Class() : WatchyGSR() { initAddOn(this); } // *** DO NOT EDIT THE CONTENTS OF THIS FUNCTION ***


// This function is required by AddOns and is used to register one or more WatchFace Styles at startup, you can add more here, but you need more variables above
// to track all of the WatchFaces and determine which ones match the CurrentGameID(), as WatchFaceGame Styles are collected and assigned at boot.

    void RegisterWatchFaces(){
      <MyFirstWatchGame>Style = AddWatchStyle("<MyFirstWatchGameName>",this,true);
    };


// This function sets up the WatchFace Games, this is required for all AddOns,
// as it sets up the Game when it is called for the first time and each time the Game is switched to from another one.

    void InsertInitWatchStyle(uint8_t StyleID){
      if (StyleID == <MyFirstWatchGame>Style){
        // Setup your variables and such for your game (read from NVS if you're using it), so when the Game mode is on, the functionb elow
        // can draw it's current state.
      }
    };

// This function is required to draw the Game on the screen, without this you'll be seeing only a blank screen.
// Be sure to read up on the various functions to allow this to happen properly.

    void InsertDrawWatchStyle(uint8_t StyleID){
      if (StyleID == <MyFirstWatchGame>Style){
        // Display your Game screen here.
        display.setFont(Design.Face.DateFont);
        display.setCursor(0,50);
        display.println("");
        display.println("Game Mode");
        display.println("Back to exit.");
      }
    };


/* This function will run on each new minute.  CurrentGameID() can be used here to separate between Games.
 * If Screen Blanking is on, this will happen 2 times an hour, 0 and 30 minutes, so account for this in your math.
 * If the Watchy is charging, then battery charging will reduce this to every 5 minutes once charging is noticed. */

    void InsertOnMinute(){
      if (GameStatusOn()) { // neglect
      } else if (WatchTime.Local.Minute == 30) GameStatus(true);  // Ask for attention for the game
    };


/* This function is used to tell WatchyGSR that you need it to stay in Active Mode (true), so you can keep the Watchy from deep sleeping.
 * CurrentGameID() can be used here to separate between Game Styles.  Use this for drawing animations on-screen, also see UpdateScreen(). */

 bool InsertNeedAwake(bool GoingAsleep) { return false; }


/* For games, when in Game Mode, all buttons pressed are returned here, it is up to the Game to HideGame() to go back to the Watch face.
 * CurrentGameID() can be used here to separate between Game Styles. */

    bool InsertHandlePressed(uint8_t SwitchNumber, bool &Haptic, bool &Refresh) {
      switch (SwitchNumber){
        case 1: //MENU
          return false;
          break;
        case 2: //Back
          GameStatus(false); // Done with the status icon on the Watch.
          HideGame();   // Done with the game, display refresh isn't needed as HideGame will cause it.
          Haptic = true;  // Cause Haptic feedback if set to true.
          return true;  // Respond with "I used a button", so the WatchyGSR knows you actually did something with a button.
          break;
        case 3: //Up
          return false;
          break;
        case 4: //Down
          return false;
      }
      return false;
    };

/* This function is useful if you want to record settings in NVS, it is called whenever the Needs Saving is set, so you can write to the NVS too.
 * A note about NVS, if the disable has been set, this will not get called to write to the NVS, please respect the NVS settings the user has set. */

    void SaveProgress(){}

};


<MyWatchGame>Class <MyWatchGame>ClassLoader;

// Put your functions here, outside of the class.
