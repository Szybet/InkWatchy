**Overridding Watch_GSR functionality**

A structure allows you to reposition AND change font color along with font for each section:

| Design Element  | Default | Description |
| --------------- | ------- | ----------- |
| Design.Menu.Top | 72 | Top of where the Menu starts vertically on the screen. |
| Design.Menu.Header | 97 | Vertical baseline of Menu Header (section). |
| Design.Menu.Data | 138 | Vertical baseline of Menu Data (value to set/change). |
| Design.Face.Time | 56 | Vertical baseline of Time on screen. |
| Design.Face.TimeHeight | 45 | Height of font used for PM indicator so it will sit at the top of TimeFont. |
| Design.Face.TimeColor | GxEPD_BLACK | Color the Time is drawn in. |
| Design.Face.TimeFont | &aAntiCorona36pt7b | Font used for Time display. |
| Design.Face.Day | 101 | Vertical baseline of Day of Week on screen. |
| Design.Face.DayColor | GxEPD_BLACK; | Color the Day of Week is drawn in. |
| Design.Face.DayFont | &aAntiCorona16pt7b; | Font used for Day of Week display. |
| Design.Face.Date | 143 | Vertical baseline of Date on screen. |
| Design.Face.DateColor | GxEPD_BLACK | Color the Date is drawn in. |
| Design.Face.DateFont | &aAntiCorona15pt7b | Font used for Date display. |
| Design.Face.Year | 186 | Vertical baseline of Year on screen. |
| Design.Face.YearColor | GxEPD_BLACK | Color the Year is drawn in. |
| Design.Face.YearFont | &aAntiCorona16pt7b | Font used for Year display. |
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

Functions available for communication:

| Function Name | Usage |
| ------------- | --------------------------------- |
| handleButtonPress(uint8_t Pressed) | Accepts Switch # from 1 to 4, can "fake" a button press. |
| float getBatteryVoltage() | Returns a cleaned battery voltage. |
| IsDark() | Is the screen currently black (Screen Off has triggered from settings). |
| VibeTo(bool Mode) | Set VibeTo to true to enable vibration motor, false to stop it. |
| MakeTime(int Hour, int Minutes, bool& Alarm) | Use a variable in Alarm set to false for normal Hour & Minutes format or true for Alarm format.  Returns a String. |
| MakeHour(uint8_t Hour) | Return the hour formatted in a String using 12 or 24hr format. |
| MakeMinutes(uint8_t Minutes) | Returns a string of the Minutes. |
| ForeColor() | Returns the current Fore (font) color for usage with current style. |
| BackColor() | Returns the current Background color for usage with current style. |
| AskForWiFi() | Tells the Watchy_GSR that your code wants WiFi, when it connects, you will see InsertWiFi() called, make sure you keep track of this yourself. |
| currentWiFi() | Returns WL_CONNECTED when connected or not, InsertWiFi() is only called when WL_CONNECTED happens. |
| endWiFi() | Tell Watchy_GSR that you're finished with the WiFi, only do this *IF* you asked for it. |

**NOTES ON WiFi**

If you plan to use WiFi, remember, users will want to actually keep using the Watchy_GSR underneath while you're using WiFi, so while it is nice to pack everything in at once, the InsertWiFi() function is repeatedly called until you tell it you're done by saying EndWiFi(), you may see another InsertWiFi() after doing so, just be sure to ignore any InserWiFi() calls you didn't ask for.

Breaking up your WiFi functions so that they're only done in parts is best.  Anything you have to wait for, make an int that tells you where you are in your work and when InsertWiFi() returns, continue where you left off.  When you're finished, make sure you EndWiFi() and make note you finished on your end.
