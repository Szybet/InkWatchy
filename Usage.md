**A simple yet pleasant looking Watchy Face, loaded with features**

![Watchy GSR Face](https://github.com/GuruSR/Watchy_GSR/blob/main/Images/Watchy_GSR.jpg)

This Watchy "face" contains the following heiarchy of options and settings:

**Legend**:  **{}** items are defaults.

|Menu | Sub-Menu Item  | Function Description |
|---- | -------------  | -------------------- |
|**Steps** | Reset Time          | Reset Steps ... Use Menu information for more help. |
|**Alarms** | Alarms #           |  HH:MM -> Full -> Days + Repeat + Active settings [^5] |
|           | Tone Repeats       |  **{Full}** repeats.  Allows you to reduce the amount of alarm tones that repeat (80%-20% in 20% increments).  Resets the tone repeats for all alarms when you change this. |
|**Timers** | Countdown Timer    | HH:MM:SS -> On/**{Off}** |
|           | Countdown Settings | **{Once}**/Repeat -> {Full} (Full, see Tone Repeats above for information).  Any changes to these reset the above Timer. |
|           | Elapsed Time       | HH:MM On/**{Off}** |
|**Options** | Watch Style       | Choose between "Classic GSR", "Ballsy" and "LCD" styles.  (Overrides can change this.) |
|            | Display Style     | **{Light}** or Dark mode. |
|            | Border Mode       | Border around the display:  **{Light}** (White) or Dark (Black). |
|            | Dexterity         | Swap the Back/Menu with UP/DOWN for left-handed users. |
|            | Menu & Back       | **{Normal}** or Swap the Menu & Back button positions. |
|            | Orientation       | Ignore button on Watchy orientation:  **{Ignore}** or Watchy UP. |
|            | Time Mode         | **{AM/PM}** or 24 Hour mode of time display. |
|            | Feedback          | **{Enable}** or Disable haptic feedback on button presses (during use). |
|            | Turbo Time        | How many seconds Watchy stays active after your last button press before sleeping. |
|            | Screen Off        | **{Disabled}**, "Always", "Bed Time" [^6], "Double Tap On", "Double Tap Only".  [^7] Screen Blanking (which updates the display every 30mins, battery savings). |
|            | Performance       | Offers **{Turbo}**, Normal and Battery Saving [^3] options, reduces responsiveness as you go away from Turbo. |
|            | WiFi Tx Power     | Allows the user to lower the default WiFi Transmission Power to save battery life during WiFi operations. |
|            | Information       | Shows the current Watchy_GSR Version and current Battery Voltage when you press "MENU". |
|            | Sync Watchy       | Sync Watchy RTC by Time, TimeZone, TimeZone & Time |
|            | Weather Interval  | (Only visible if Watchface asks for Weather) Set the interval between Weather requests. |
|            | Watchy Connect    | Used to give the WiFi "X" credentials to Watchy.  "X" WiFi is the last "good" connected WiFi. "BACK" to immediately exit. |
|            | OTA Update        | Used with Arduino (and platformio) to upload a compile to Watchy via WiFi.  (ESCAPE by holding "BACK" for 10 seconds.) |
|            | OTA Website       | Website offers Backup & Restore of Settings, WiFi AP Settings and WiFi OTA upload of a bin file and OpenWeatherMap API Key setting.  (ESCAPE by holding "BACK" for 10 seconds.) |
|**Troubleshoot** | Reset Screen     | Reset screen if artifacting or ghosting is happening. |
|                 | Watchy Reboot    | Reboot the Watchy in the event something stops working. |
|                 | Edit RTC         | Detect drift in RTC clock (requires 2 edits one to start, one at battery low level). Excessive drift enters non-RTC mode. [^1] |
|                 | Storage Settings | Allows you to disable Non-Volatile Storage of settings for this Watchy face (or re-enable them). [^4] |
            
Button usage:

- "Menu" (SW1):  Activates/Toggles items, for Alarms, Timers and Step, advances forward where HH:MM:SS is shown.  In Turbo Time increases seconds.
- "Back" (SW2):  Backs out of an item or backs up to previous section when in HH:MM:SS areas.
-  "UP"  (SW3):  Moves menu selection up.  When inside HH:MM it increases selected value (these cycle if above max value).
- "DOWN" (SW4):  Moves menu selection down.  When inside HH:MM it decrease selected value (these also cycle if below 0).

STEPS:

- Steps holds the current steps and (Yesterday's steps if any).
- Reset Time (HH:MM) represents the time of day where the Step counter is automatically reset and placed in the "Yesterday"'s list.
- Reset Steps will reset the current steps but will *NOT* move them to "Yesterday".

ALARMS:  (WHITE settings means "ON", Black is "OFF")

- Alarms allow HH:MM setting [^5]
- Tone Repeats are custom per Alarm but are reset to the value you use in Tone Repeats.
- All days of the week can be toggled On/**{Off}**.  White days are the only active days.
- Repeat offers to repeat the choices.  If Repeat is **{off}**, day will go Black.
- Active sets the alarm as operational or **{not}**.

TIMERS:

- Countdown Timer will count down from the HH:MM:SS you set.  On/Off will reset the HH:MM when pressed. [^8]
- Elapsed Timer will count up when started (On) and will stop **{Off}** when stopped.  Starting again resets to 00:00:00. [^8]

ALARMS & COUNTDOWN TIMER PULSES:

- ALARM TONES:
  1.  Large solid pulse.  (LOUD)
  2.  2 smaller pulses.   (NOISY)
  3.  3 tiny pulses.  (QUIET)
  4.  2 very tiny pulses, a very tiny pause, 2 more very tiny pulses.  (VERY QUIET, useful in meetings)

- COUNTDOWN:  6 very tiny pulses.

These will cycle in a loop from ALARM 1 to COUNTDOWN playing their tones until they run out.  All 5 will not last longer than 50 seconds.  To stop them from playing, edit their time.  Each Alarm (and the Countdown Timer) have their own reduction for repeats of their tones, which will shorten these durations.

SCREEN OFF:

1.  **5** modes are present, **{Disabled}**, "Always", "Bed Time", "Double Tap On" and "Double Tap Only".
2.  "Always" mode will turn the screen off when not in a menu, it will go off after the delay set in "Screen Auto-Off" (MENU once from "Screen Off"). [^2]
3.  "Bed Time" mode will turn the screen off at any time between the two hours, inside or outside of a menu.  MENU after "Screen Auto-Off" to see the "Sleeping Begins".
4.  "Sleeping Begins" and "Sleeping Ends" cannot be the same hour, but can be 1 hour apart from each other, either before or after the other.
5.  All modes when not **Disabled** will use the "Screen Auto-Off" delay after any button press.
6.  While the screen is off, any button press will just turn it on for the duration set in "Screen Auto-Off", a second press is needed to use the Watchy as normal.
7.  If Turbo Time is set longer than "Screen Auto-Off", Turbo Time will still be active and buttons will react as normal with respect to Screen Blanking.
8.  "Double Tap On" will allow buttons to turn the screen on or a double tap on the device.
9.  "Double Tap Only" will only allow a double tap to turn the screen on, only then will buttons work.  "Screen Auto-Off" will default to 2 seconds if off.

FIRST TIME USAGE:

1.  Go into Options -> Watchy Connect -> MENU to Connect.
2.  Connect to the Watchy Connect WiFi access point with your phone, laptop, computer, etc.  (Password is:  Watchy123)
3.  Open a web browser and go to the IP address on the Watchy, once there you need to choose "Edit Additional WiFi Access Points".  `See WiFi USAGE for more.`
4.  Add all of your known WiFi access point names and passwords in here and click Store.
5.  Hit the BACK button on the Watchy to back out, now go to OTA Website and click MENU to get the Watchy to connect to your WiFi, it should if you entered the password correctly in 3, if not, repeat the process by using UP to get to Watchy Connect and pushing MENU and repeating from step 2.
6.  (Optional) Restoring settings with Watchy Connect OR OTA Website, select "Backup & Restore Settings" if you have previous Watchy settings to restore, paste in the settings you had and click Store.

WiFi USAGE:

WiFi now has 10 entries (AP A to AP J), the most recent successfully connected AP is labelled "X".  See compilation for usage for making a default "X".

WiFi will require the Performance to boost to Turbo during it's usage, limit usage to reduce battery drain.

WiFi entries can be edited from Watchy's Options -> OTA Website, surf to the Watchy in a browser and select "Edit Additional WiFi Access Points".  When they are correct, click "Store".  The "?" at the end of each password, lets you toggle the *'s on/off.

WiFi options in the Options menu will not be available if the low battery indicator is on.

[^1]:  Edit RTC has "Use RTC" (enabled/disabled) and "Drift Management" (to start Drift calculation) and when you do it again when the battery is low, you'll see Edit RTC to set the time right again, then "Calculate Drift", you'll then be greeted with a number /(+-)1.  Up will let you start the "Drift Management" again or see the current seconds between corrected second (+/-) to keep the RTC in time.  During "Drift Management" all NTP requests will go unanswered, until "Calculate Drift" is done, but should be done as late in the battery life as possible to maximize calculation accuracy.
[^2]:  "Always" mode is used during non-RTC mode, with the exception that menus are ignored (acting like "Bed Time" mode).
[^3]:  "Battery Saving" is forced on when using non-RTC mode.
[^4]:  Disabling Non-Volatile Storage will ask for you to select you want to proceed, if you proceed the NVS will have the settings removed, Watchy will reboot and will continue to not store any settings in the NVS.
[^5]:  With the invention of SmallRTC's atMinuteWake, all Screen Blanking will now use atMinuteWake for on the hour (0 minute) and 30 minutes past the hour, Alarms will now *WORK* in all Screen Blanking modes and will properly fire.  The only change is, the Countdown Timer will *NOT* allow the atMinuteWake to happen, but normal nextMinuteWakes until it finishes.  SmallRTC's atMinuteWake will cause the Watchy to only wake 2 (plus Alarms) times an hour, massively reducing battery usage.
[^6]:  Double Tap works while in Bed Time, as do the buttons.
[^7]:  While your wrist is "Up", the Watchy will sit in Active Mode (battery usage), until you lower or tilt your wrist to cause the screen to go out again.
[^8]:  In non-RTC mode, timers will not display seconds correctly, Countdown Timer will ignore seconds completely.
