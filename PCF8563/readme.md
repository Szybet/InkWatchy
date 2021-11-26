**The following are present:**

**Library:**

- This contains the library modified from SQFMI's version, removed the char output functions that return formatted time as I felt it didn't need to be in a hardware library, a Watch face yes, library for hardware, I didn't think so.
- This library is mostly a "direct plug in" in replacing the DS3232 RTC, meaning using the following code can make your Watch Face compilable for either by just setting 1 define.
- Locate the DS3232RTC items in both files below and replace the line with the section.

Class (Watchy.h):

```
#ifndef PCF8563RTC
        static DS3232RTC RTC;
#else
        static PCF8563 RTC;
#endif
```

Class (Watchy.cpp):

```
#ifndef PCF8563RTC
DS3232RTC WatchyGSR::RTC(false); 
#else
PCF8563 WatchyGSR::RTC(false);
#endif
```

- The library has 1 notable #define TIME_H_DIFF which is used to put the year right from 1970 to 2000.

**Usage:**

- Set a define in "Watchy".h for PCF8563RTC to use it, or don't to use the original DS3232RTC.
- Place the PCF8563 folder anywhere in the compiler's range of view for finding files to compile.
- Report any linking errors to the Compilation Instructions file, so I can track them (also comment on which IDE you used).
