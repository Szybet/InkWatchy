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
