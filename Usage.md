**A simple yet pleasant looking Watchy Face, loaded with features**

![Watchy GSR Face](https://github.com/GuruSR/Watchy_GSR/blob/main/Images/Watchy_GSR.jpg)

This Watchy "face" contains the following heiarchy of options and settings:

**Legend**:  **{}** items are defaults.

|Menu | Sub-Menu Item  | Function Description |
|---- | -------------  | -------------------- |
|**Steps** | Reset Time         | Reset Steps ... Use Menu information for more help. |
|**Alarms** | Alarms #          |  HH:MM -> Full -> Days + Repeat + Active settings [^5] |
|           | Tone Repeats      |  **{Full}** repeats.  Allows you to reduce the amount of alarm tones that repeat (80%-20% in 20% increments).  Resets the tone repeats for all alarms when you change this. |
|**Timers** | Countdown Timer   | HH:MM -> {Full} -> On/**{Off}**  (Full, see Tone Repeats above for information). |
|           | Elapsed Time      | HH:MM On/**{Off}** |
|**Options** | Watch Style      | Choose between "Classic GSR" and "Ballsy" style.  (Overrides can change this.) |
|            | Display Style    | **{Light}** or Dark mode. |
|            | Border Mode      | Border around the display:  **{Light}** (White) or Dark (Black). |
|            | Dexterity        | Swap the Back/Menu with UP/DOWN for left-handed users. |
|            | Menu & Back      | **{Normal}** or Swap the Menu & Back button positions. |
|            | Orientation      | Ignore button on Watchy orientation:  **{Ignore}** or Watchy UP. |
|            | Time Mode        | **{AM/PM}** or 24 Hour mode of time display. |
|            | Feedback         | **{Enable}** or Disable haptic feedback on button presses (during use). |
|            | Turbo Time       | How many seconds Watchy stays active after your last button press before sleeping. |
|            | Screen Off       | **{Disabled}**, "Always", "Bed Time" [^6], "Double Tap On", "Double Tap Only".  [^7] Screen Blanking (which uses no cpu to update display, battery savings). |
|            | Performance      | Offers **{Turbo}**, Normal and Battery Saving [^3] options, reduces responsiveness as you go away from Turbo. |
|            | WiFi Tx Power    | Allows the user to lower the default WiFi Transmission Power to save battery life during WiFi operations. |
|            | Information      | Shows the current Watchy_GSR Version and current Battery Voltage when you press "MENU". |
|            | Sync Watchy      | Sync Watchy RTC by Time, TimeZone, TimeZone & Time |
|            | Watchy Connect   | Used to give the WiFi "X" credentials to Watchy.  "X" WiFi is the last "good" connected WiFi. "BACK" to immediately exit |
|            | OTA Update       | Used with Arduino (and platformio) to upload a compile to Watchy via WiFi.  (ESCAPE by holding "BACK" for 10 seconds.) |
|            | OTA Website      | Website offers Backup & Restore of Settings, WiFi AP Settings and WiFi OTA upload of a bin file.  (ESCAPE by holding "BACK" for 10 seconds.) |
|**Troubleshoot** | Reset Screen     | Reset screen if artifacting or ghosting is happening. |
|                 | Watchy Reboot    | Reboot the Watchy in the event something stops working. |
|                 | Detect Drift     | Detect drift in RTC clock (takes 2 minutes). Excessive drift enters non-RTC mode. [^1] |
|                 | Storage Settings | Allows you to disable Non-Volatile Storage of settings for this Watchy face (or re-enable them). [^4] |
            
Button usage:

- "Menu" (SW1):  Activates/Toggles items, for Alarms, Timers and Step, advances forward where HH:MM is shown.  In Turbo Time increases seconds.
- "Back" (SW2):  Backs out of an item or backs up to previous section when in HH:MM areas.
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

- Countdown Timer will count down from the HH:MM you set.  On/Off will reset the HH:MM when pressed.
- Elapsed Timer will count up when started (On) and will stop **{Off}** when stopped.  Starting again resets to 00:00.

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
3.  Scan for your home network (or whichever one you want to use it with) and setup the WiFi connectivity there, once it works, the Access Point will close.  This will ALWAYS ask for a new WiFi setting, unlike previous versions, you can change your WiFi "X" (last known good connection).
4.  Go to (Options -> ) OTA Website, browse to the IP address in Watchy menu.
5.  Pick "Backup & Restore Settings" if you have previous Watchy settings to restore, paste in the settings you had and click Store.
6.  If you don't have previous settings, go into "Edit Additional WiFi Access Points" and add up to 10 there, click Store to finish.  (See WiFi USAGE for more).

WiFi USAGE:

WiFi now has 10 entries (AP A to AP J), the most recent successfully connected AP is labelled "X".  See compilation for usage for making a default "X".

WiFi will require the Performance to boost to Turbo during it's usage, limit usage to reduce battery drain.

WiFi entries can be edited from Watchy's Options -> OTA Website, surf to the Watchy in a browser and select "Edit Additional WiFi Access Points".  When they are correct, click "Store".  The "?" at the end of each password, lets you toggle the *'s on/off.

WiFi options in the Options menu will not be available if the low battery indicator is on.

[^1]:  If non-RTC mode is entered, the entry for "Detect Drift" will be replaced with "Return to RTC" when backed away from the results, which will state the RTC as bad.  The Watchy will then turn off a variety (and some will be locked) of settings to ensure better battery life, reducing timer and alarm announcements and will force the screen to blank unless you press a button.  While non-RTC mode is active, menus will not keep the screen on (constant "Bed Time" mode), so be sure to increase your "Screen Auto-Off" to adjust it's frequency.
[^2]:  "Always" mode is used during non-RTC mode, with the exception that menus are ignored (acting like "Bed Time" mode).
[^3]:  "Battery Saving" is forced on when using non-RTC mode.
[^4]:  Disabling Non-Volatile Storage will ask for you to select you want to proceed, if you proceed the NVS will have the settings removed, Watchy will reboot and will continue to not store any settings in the NVS.
[^5]:  With the invention of SmallRTC's atMinuteWake, all Screen Blanking will now use atMinuteWake for on the hour (0 minute) and 30 minutes past the hour, Alarms will now *WORK* in all Screen Blanking modes and will properly fire.  The only change is, the Countdown Timer will *NOT* allow the atMinuteWake to happen, but normal nextMinuteWakes until it finishes.  SmallRTC's atMinuteWake will cause the Watchy to only wake 2 (plus Alarms) times an hour, massively reducing battery usage.
[^6]:  Double Tap works while in Bed Time, as do the buttons.
[^7]:  While your wrist is "Up", the Watchy will sit in Active Mode (battery usage), until you lower or tilt your wrist to cause the screen to go out again.
