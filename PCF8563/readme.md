**The following are present:**

**Library:**

- This contains the library modified from SQFMI's version, removed the char output functions that return formatted time as I felt it didn't need to be in a hardware library, a Watch face yes, library for hardware, I didn't think so.
- This library is mostly a "direct plug in" in replacing the DS3232 RTC, meaning using the following code can make your Watch Face compilable for either by just setting 1 define.

Class (Watchy.h):

#ifndef PCF8563RTC
        static DS3232RTC RTC;
#else
        static PCF8563 RTC;
#endif

Class (Watchy.cpp):

#ifndef PCF8563RTC
DS3232RTC WatchyGSR::RTC(false); 
#else
PCF8563 WatchyGSR::RTC(false);
#endif

- The library has 2 notable #defines, PCF8563RTC (defined uses it, commented out, doesn't).  Also TIME_H_DIFF which is used to put the year right from 1970 to 2000.

**beta:**

- These files go over top of the current build, these files offer compiling the Watchy_GSR with the PCF8563 RTC as in Watchy_GSR.h, it is set to 1.
- Move Watchy.h and Watchy.cpp from the libraries section, out to it's own Stream, so it isn't in the way of compiling this beta.
