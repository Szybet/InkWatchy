**Version 1.1:**
- **FIX:**  Repaired Watchy Sync menu to properly work with TimeZone requests.
- **FIX:**  Repaired incorrect Menu Item (Steps) showing up after Watchy Sync was used.
- **ADD:**  Updated icons for WiFi, TimeZone & NTP.  WiFi icon now shows -AP for Access Point mode, -X for last good WiFi (connected) and (AP) -A through -J from Additional WiFi Access Points page.
- **ADD:**  Tone reductions to Alarms and Countdown Timer.
- **ADD:**  Tone Reduction in Alarms Sub-Menu to set all 4 alarms to this reduction.
- **ADD:**  Manual WiFi default settings in Defines_GSR.h (WiFi_DEF_SSID) for people who want to manually add their always default WiFi at compile time.
- **FIX:**  Move NTP test while charging is seen to detect Battery which only happens once a minute (avoids constant NTP checks blocking WiFi usage when no WiFi is available).

**Version 1.2:**
- **FIX:**  Fixed Additional WiFi Access Point functionality.
- **FIX:**  Fixed minor issues with various menus not giving proper results.
- **FIX:**  Fixed WiFi indicator to show properly.
- **FIX:**  Fixed POSIX Timezone to be null when not set with a proper one, will auto-force Timezone detection on reboot only if none is stored in Non-Volatile Storage.
- **ADD:**  Non-Volatile Storage for settings and Timezone.
- **ADD:**  Troubleshooting Menu with Screen Reset, Watchy Reboot and Detect Travel (RTC).
- **ADD:**  Feature Add:  Screen Off with "Disabled", "Always" and "Bed Time".

**Version 1.2.1:**
- **FIX:**  Made buttons work always with screen blanking, once to bring screen on, second one does action (if one).
- **FIX:**  Fixed Restore Settings with empty data causing Watchy to crash.
- **FIX:**  Detect Drift not saving changed value to NVS.
- **FIX:**  Detect Drift now correctly checks 1 minute.  (No more oddities with drift calculations.)

**Version 1.3:**
- **ADD:**  Excessive failover for Detect Travel +/- 30 seconds will cause Watchy to ignore RTC and use internal CPU timing [see Usage]
- **ADD:**  CPU control to improve lifespan of battery operation.  8+ hours of battery while in non-RTC mode, using tailored option changes.
- **FIX:**  Fix WiFi timeouts not properly setup if inside Active Mode for too long.
- **FIX:**  Status printing not showing up on dark backgrounds.
- **FIX:**  Fixed Dark so the face drawing knows it happened and vice-versa.

**Version 1.3.1:**
- **ADD:**  Added Performance menu with "Turbo", "Normal" and "Battery Saving" options.
- **FIX:**  Fixed CPU control to be less intensive with respect to reading the CPU too much, now happens at top of init once, using valid set response to record value later.

**Version 1.3.2:**
- **FIX:**  Fixed Turbo Mode and Sleep Modes to not work on each minute regardless of you pushing a button or not.

**Version 1.3.3:**
- **FIX:**  Fixed the restoration of Screen Blanking from storage and restoration from OTA Website.

**Version 1.3.4:**
- **ADD:**  Unification of 2 Watchy versions (DS3231 & PCF8563), also included (and noted for submission) WatchyBattery.h to read the proper ADC PIN depending on RTC type.

**Version 1.3.5:**
- **FIX:**  Fixed the "Bed Time" orientation to only work during the Bed Time range.
- **ADD:**  Double Tap modes "On" and "Only".
- **ADD:**  3 new repositories added with branched out functions:  SmallRTC (replacement of WatchyRTC), SmallNTP (industry standard NTP) and Olson2POSIX Timezone collection and setting.
- **FIX:**  Fixed battery level control of Watchy, extremely low battery will stop updating the screen to prolong battery life, button presses will only work.

**Version 1.3.6:**
- **FIX:**  Corrected Timezone tests (they were backwards).
- **FIX:**  Changed border from "Show/Hide" to "Light/Dark" to match Display Style.
- **FIX:**  Updated BMA control to properly work with Double Tap, method is there for Wrist tilt, no motion setup for it at present.
- **ADD:**  Functionality to block Non-Volatile Storage from being used, will erase and reboot the Watchy and stop using it for this Watchy face.
- **FIX:**  Corrected NTP server to pool.ntp.org, it should work globally, though it does have a restriction on use, so expect it to fail if used too much.

**Version 1.3.7:**
- **ADD:**  Unified format of main Watchy face, so override can be done to include new fonts and relocation.
- **ADD:**  Functions to allow insertion of override code into certain parts of Watchy_GSR without having to replicate whole sections.
- **FIX:**  Changed icons.h to Icons_GSR.h to remove Watchy.h conflict and also removed steps image.

**Version 1.3.8:**
- **ADD:**  Extra Design functionality for Unified format of main Watchy face which includes orientation of elements.
- **FIX:**  InsertWiFi() now only runs if no other process is using the WiFi to avoid collsions.

**Version 1.3.9:**
- **FIX:**  Repaired Screen Blanking issues with alarms during Bed Time (utilizing atMinuteWake from SmallRTC 1.5).
- **FIX:**  Recognize Countdown Timer is in use during Screen Blanking and not use atMinuteWake, but nextMinuteWake instead.
- **ADD:**  All Screen Blanking now use atMinuteWake when Countdown Timer is not in use, this works by waking at minutes 0 and 30 or the next Alarm minute.
- **FIX:**  makeTime and breakTime introduce a month and week day starting with 1, which is incorrect, versions from SmallRTC & SmallNTP now fix this issue, so January 29 will not be seen as February 29 by the RTCs.

**Version 1.4:**
- **FIX:**  Corrected the orientation of the display with the accelerometer.
- **ADD:**  Added Wrist Tilt support for all Screen Blanking with "Screen on Hold" (actively watches your wrist to turn screen off again, heavy battery usage).

**Version 1.4.1:**
- **FIX:**  Corrected missing internal wake functions.
- **ADD:**  Watch Style menu with 1 extra style called "Ballsy".
- **FIX:**  Re-arranged Design Menu setup and included Font.

**Version 1.4.2:**
- **FIX:**  Repaired the Tilt detection so non-Screen Blanking won't keep the Watchy in Active Mode draining the battery while upright.
- **ADD:**  Extra functions (see Override Information) to offer adding or replacing existing Watch Styles without altering the base code.
