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
