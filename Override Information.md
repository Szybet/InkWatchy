**Overridding Watch_GSR functionality**

A structure allows you to reposition AND change font color along with font for each section:

| Design Element  | Default | Description |
| --------------- | ------- | ----------- |
| Design.Menu.Top | 72 | Top of where the Menu starts vertically on the screen. |
| Design.Menu.Header | **25** | Vertical baseline of Menu Header (section) **from Design.Menu.Top**. |
| Design.Menu.Data | **66** | Vertical baseline of Menu Data (value to set/change) **from Design.Menu.Top**. |
| Design.Menu.Font | &aAntiCorona12pt7b | Font used for Menu Header & Data display. |
| Design.Face.Time | 56 | Vertical baseline of Time on screen. |
| Design.Face.TimeHeight | 45 | Height of font used for PM indicator so it will sit at the top of TimeFont. |
| Design.Face.TimeColor | GxEPD_BLACK | Color the Time is drawn in. |
| Design.Face.TimeFont | &aAntiCorona36pt7b | Font used for Time display. |
| Design.Face.TimeLeft | {Unused} | Left point to start drawing Time display. |
| Design.Face.TimeStyle | WatchyGSR::dCENTER | Alignment method for Time display.  If set for WatchyGSR:dRIGHT, AM/PM indicator will be on the left. |
| Design.Face.Day | 101 | Vertical baseline of Day of Week on screen. |
| Design.Face.DayColor | GxEPD_BLACK; | Color the Day of Week is drawn in. |
| Design.Face.DayFont | &aAntiCorona16pt7b | Font used for Day of Week display. |
| Design.Face.DayLeft | {Unused} | Left point to start drawing Day of Week display. |
| Design.Face.DayStyle | WatchyGSR::dCENTER | Alignment method for Day of Week display. |
| Design.Face.Date | 143 | Vertical baseline of Date on screen. |
| Design.Face.DateColor | GxEPD_BLACK | Color the Date is drawn in. |
| Design.Face.DateFont | &aAntiCorona15pt7b | Font used for Date display. |
| Design.Face.DateLeft | {Unused} | Left point to start drawing Date display. |
| Design.Face.DateStyle | WatchyGSR::dCENTER | Alignment method for Date display. |
| Design.Face.Year | 186 | Vertical baseline of Year on screen. |
| Design.Face.YearColor | GxEPD_BLACK | Color the Year is drawn in. |
| Design.Face.YearFont | &aAntiCorona16pt7b | Font used for Year display. |
| Design.Face.YearLeft | {Unused} | Left point to start drawing Year display. |
| Design.Face.YearStyle | WatchyGSR::dCENTER | Alignment method for Year display. |
| Design.Status.WIFIx | 5 | Left edge of WIFI status on screen. |
| Design.Status.WIFIy | 193 | Vertical baseline of WIFI status on screen. |
| Design.Status.BATTx | 155 | Left edge of Battery state on screen. |
| Design.Status.BATTy | 178 | Vertical baseline of Battery state on screen. |

Functions available for overriding:

| Function Name | Usage |
| ------------- | --------------------------------- |
| showWatchFace() | Override to change the entire WatchFace drawing. |
| drawWatchFace() | Override to change the entire drawing of the WatchFace except screen refreshing. |
| drawTime() | Override to change the format of how the Time is drawn. |
| drawDay() | Override to change the format of how the Day of Week is drawn. |
| drawDate() | Override to change the format of how the Date is drawn. |
| drawYear() | Override to change the format of how the Year is drawn. |
| drawChargeMe() | Override to change the format of how the Battery Status is drawn. |
| drawStatus() | Override to change the format of how the current WiFi Status is drawn. |
 
Functions for inserting extra code in places.

| Function Name | Usage |
| ------------- | --------------------------------- |
| InsertPost() | This Function offers a post "boot" insert, so you can make changes after settings are loaded. |
| InsertBitmap() | This Function allows you to place a bitmap on the Watchy face before anything is drawn on it. |
| InsertDefaults() | This Function is done at the end of setupDefaults(), so you can add your own defaults. |
| InsertOnMinute() | This Function is called once the Clock has been updated to the new minute but before the screen is drawn. |
| InsertWiFi() | This Function is called repeatedly in a loop only *IF* WiFi has been enabled and connected, only use this if you asked for it. |
| InsertWiFiEnding() | This Function is called when WiFi has been turned off. |
| InsertAddWatchStyles() | Use this function to add Watch Styles starting from Index 0 [`AllowDefaultWatchStyles(false)`] or from Index 2 if allowing default Watch Styles. |
| InsertInitWatchStyle() | The init function as seen at the end of Watchy_GSR.cpp.  See **Version 1.4.2 Additions** below. |
| InsertDrawWatchStyle() | The Draw function as seen at the bottom of Watchy_GSR.cpp. See **Version 1.4.2 Additions** below. |
 
Functions available for communication:

| Function Name | Usage |
| ------------- | --------------------------------- |
| handleButtonPress(uint8_t Pressed) | Accepts Switch # from 1 to 4, can "fake" a button press. |
| float getBatteryVoltage() | Returns a cleaned battery voltage. |
| IsDark() | Is the screen currently black (Screen Off has triggered from settings). |
| VibeTo(bool Mode) | Set VibeTo to `true` to enable vibration motor, `false` to stop it. |
| MakeTime(int Hour, int Minutes, bool& **Alarm**) | Use a variable in **Alarm** parameter always.  When **Alarm** is set to `false` you'll get normal Hour & Minutes format based on Options, **Alarm** will be `true` for PM.  Setting **Alarm** to `true` for Alarm format.  Returns a String. |
| MakeHour(uint8_t Hour) | Return the hour formatted in a String using 12 or 24hr format. |
| MakeMinutes(uint8_t Minutes) | Returns a string of the Minutes. |
| ForeColor() | Returns the current Fore (font) color for usage with current style. |
| BackColor() | Returns the current Background color for usage with current style. |
| AskForWiFi() | Tells the Watchy_GSR that your code wants WiFi, when it connects, you will see InsertWiFi() called, make sure you keep track of this yourself. |
| currentWiFi() | Returns WL_CONNECTED when connected or not, InsertWiFi() is only called when WL_CONNECTED happens. |
| endWiFi() | Tell Watchy_GSR that you're finished with the WiFi, only do this *IF* you asked for it. |
| AllowDefaultWatchStyles(bool Allow) | Will state if you want (**{true}**/false) the original Watch Styles (Index 0 (Classic GSR) and 1 (Ballsy) to be used first). |
| AddWatchStyle(String StyleName) | Will return the Index of the added Watch Style (255 = error), 30 character max limit on Watch Style name. |

**NOTES ON WiFi**

If you plan to use WiFi, remember, users will want to actually keep using the Watchy_GSR underneath while you're using WiFi, so while it is nice to pack everything in at once, the `InsertWiFi()` function is repeatedly called until you tell it you're done by saying `EndWiFi()`, you may see another `InsertWiFi()` after doing so, just be sure to ignore any `InsertWiFi()` calls you didn't ask for.

Breaking up your WiFi functions so that they're only done in parts is best.  Anything you have to wait for, make an int that tells you where you are in your work and when `InsertWiFi()` returns, continue where you left off.  When you're finished, make sure you `EndWiFi()` and make note you finished on your end by zeroing your int.

Recommend an int variable being set to 1 when you `AskForWiFi()`, then when `InsertWiFi()` is called, you do part of the work, ++ the int, when it returns again, repeat until you're finished.  When you finish, call `EndWiFi()` and set your int to 0.  When you receive an `InsertWiFiEnd()`, set your int to 0, this way, your code will always work properly.  You can even test in `InsertOnMinute()` if your int is 0 before commencing so you don't try twice.  Only `AskForWiFi()` once and only call `EndWiFi()` once or any other operation using WiFi may be cancelled.

| TIME Structure | Contents |
| -------------------- | ------------------ |
| WatchTime.Local.Second | Contains the current second(s) in Local time. |
| WatchTime.Local.Minute | Contains the current minute(s) in Local time. |
| WatchTime.Local.Hour | Contains the current Hour in Local time. |
| WatchTime.Local.Wday | Contains the Day of Week (Days since Sunday) in Local time. |
| WatchTime.Local.Day | Contains the Date (1 to 31) in Local time. |
| WatchTime.Local.Month | Contains the Month (1 to 12) in Local time. |
| WatchTime.Local.Year | Contains the Year (since 1900) in Local time. |
| | |
| WatchTime.UTC.Second | Contains the current second(s) in Coordinated Universal time. |
| WatchTime.UTC.Minute | Contains the current minute(s) in Coordinated Universal time. |
| WatchTime.UTC.Hour | Contains the current Hour in Coordinated Universal time. |
| WatchTime.UTC.Wday | Contains the Day of Week (Days since Sunday) in Coordinated Universal time. |
| WatchTime.UTC.Day | Contains the Date (1 to 31) in Coordinated Universal time. |
| WatchTime.UTC.Month | Contains the Month (1 to 12) in Coordinated Universal time. |
| WatchTime.UTC.Year | Contains the Year (since 1970) in Coordinated Universal time. |

**Version 1.4.2 Additions**

The addition of the following functions will allow overriding both the original Watch Styles or adding to them, the author's choice.  This is all done without writing any changes to the Watchy_GSR base code, so as to avoid having to re-apply changes to that code after version changes.

Overriding `InsertDefaults()` function, you can call this function:

`AllowDefaultWatchStyles({true}/false);` // Will state if you want the original Watch Styles (Index 0 (Classic GSR) and 1 (Ballsy) to be used first).

Just after the default Watch Styles are added (if told to), another function will be called:

`InsertAddWatchStyles()` // Use this function to add Watch Styles starting from Index 0 [`AllowDefaultWatchStyles(false)`] or from Index 2 if allowing default Watch Styles.

`uint8_t AddWatchStyle(string StyleName)` // Will return the Index of the added Watch Style (255 = error), 30 character max limit on Watch Style name.

Override these two functions to add your Init and Draw for the Watch Styles:

`void InsertInitWatchStyle(uint8_t StyleID)`  // The init function as seen at the end of Watchy_GSR.cpp.

`void InsertDrawWatchStyle(uint8_t StyleID)`  // The Draw function as seen at the bottom of Watchy_GSR.cpp.

Overriding in this manner can be done using a switch and only checking for your Indexes, but you could also just as easily use an IF statement and record your Watch Styles in uint8_t variables for each during `InsertAddWatchStyles()`.  The best part about this is, all of this is done PRIOR to the settings reload from NVS (if not disabled), so your chosen Watch Style will remain after a reboot.
