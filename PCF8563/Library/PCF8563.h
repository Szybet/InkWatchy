/*****
 *  NAME
 *    Pcf8563 Real Time Clock support routines
 *  AUTHOR
 *    Joe Robertson, jmr
 *    orbitalair@bellsouth.net
 *    http://orbitalair.wikispaces.com/Arduino
 *  CREATION DATE
 *    9/24/06,  init - built off of usart demo.  using mikroC
 *  NOTES
 *  HISTORY
 *    10/14/06 ported to CCS compiler, jmr
 *    2/21/09  changed all return values to hex val and not bcd, jmr
 *    1/10/10  ported to arduino, jmr
 *    2/14/10  added 3 world date formats, jmr
 *    28/02/2012 A. Pasotti
 *             fixed a bug in RTCC_ALARM_AF,
 *             added a few (not really useful) methods
 *    22/10/2014 Fix whitespace, tabs, and newlines, cevich
 *    22/10/2014 add voltLow get/set, cevich
 *    22/10/2014 add century get, cevich
 *    22/10/2014 Fix get/set date/time race condition, cevich
 *    22/10/2014 Header/Code rearranging, alarm/timer flag masking,
 *               extern Wire, cevich
 *    26/11/2014 Add zeroClock(), initialize to lowest possible
 *               values, cevich
 *    22/10/2014 add timer support, cevich
 *
 *  TODO
 *    x Add Euro date format
 *    Add short time (hh:mm) format
 *    Add 24h/12h format
 ******
 *  Robodoc embedded documentation.
 *  http://www.xs4all.nl/~rfsber/Robo/robodoc.html
 */

#ifndef PCF8563_H
#define PCF8563_H

/* the read and write values for pcf8563 rtcc */
/* these are adjusted for arduino */
#define RTCC_R      0xa3
#define RTCC_W      0xa2

#define RTCC_SEC        1
#define RTCC_MIN        2
#define RTCC_HR         3
#define RTCC_DAY        4
#define RTCC_WEEKDAY    5
#define RTCC_MONTH      6
#define RTCC_YEAR       7
#define RTCC_CENTURY    8

/* register addresses in the rtc */
#define RTCC_STAT1_ADDR     0x0
#define RTCC_STAT2_ADDR     0x01
#define RTCC_SEC_ADDR       0x02
#define RTCC_MIN_ADDR       0x03
#define RTCC_HR_ADDR        0x04
#define RTCC_DAY_ADDR       0x05
#define RTCC_WEEKDAY_ADDR   0x06
#define RTCC_MONTH_ADDR     0x07
#define RTCC_YEAR_ADDR      0x08
#define RTCC_ALRM_MIN_ADDR  0x09
#define RTCC_SQW_ADDR       0x0D
#define RTCC_TIMER1_ADDR    0x0E
#define RTCC_TIMER2_ADDR    0x0F

/* setting the alarm flag to 0 enables the alarm.
 * set it to 1 to disable the alarm for that value.
 */
#define RTCC_ALARM          0x80
#define RTCC_ALARM_AIE      0x02
#define RTCC_ALARM_AF       0x08
/* optional val for no alarm setting */
#define RTCC_NO_ALARM       99

#define RTCC_TIMER_TIE      0x01  // Timer Interrupt Enable

#define RTCC_TIMER_TF       0x04  // Timer Flag, read/write active state
                                  // When clearing, be sure to set RTCC_TIMER_AF
                                  // to 1 (see note above).
#define RTCC_TIMER_TI_TP    0x10  // 0: INT is active when TF is active
                                  //    (subject to the status of TIE)
                                  // 1: INT pulses active
                                  //    (subject to the status of TIE);
                                  // Note: TF stays active until cleared
                                  // no matter what RTCC_TIMER_TI_TP is.
#define RTCC_TIMER_TD10     0x03  // Timer source clock, TMR_1MIN saves power
#define RTCC_TIMER_TE       0x80  // Timer 1:enable/0:disable

/* Timer source-clock frequency constants */
#define TMR_4096HZ      B00000000
#define TMR_64Hz        B00000001
#define TMR_1Hz         B00000010
#define TMR_1MIN        B00000011

#define RTCC_CENTURY_MASK   0x80
#define RTCC_VLSEC_MASK     0x80

/* date format flags */
#define RTCC_DATE_WORLD     0x01
#define RTCC_DATE_ASIA      0x02
#define RTCC_DATE_US        0x04
/* time format flags */
#define RTCC_TIME_HMS       0x01
#define RTCC_TIME_HM        0x02

// Alarm masks
enum ALARM_TYPES_t {
    ALM1_EVERY_SECOND = 0x0F,
    ALM1_MATCH_SECONDS = 0x0E,
    ALM1_MATCH_MINUTES = 0x0C,     // match minutes *and* seconds
    ALM1_MATCH_HOURS = 0x08,       // match hours *and* minutes, seconds
    ALM1_MATCH_DATE = 0x00,        // match date *and* hours, minutes, seconds
    ALM1_MATCH_DAY = 0x10,         // match day *and* hours, minutes, seconds
    ALM2_EVERY_MINUTE = 0x8E,
    ALM2_MATCH_MINUTES = 0x8C,     // match minutes
    ALM2_MATCH_HOURS = 0x88,       // match hours *and* minutes
    ALM2_MATCH_DATE = 0x80,        // match date *and* hours, minutes
    ALM2_MATCH_DAY = 0x90,         // match day *and* hours, minutes
};

// Square-wave output frequency (TS2, RS1 bits)
#define SQW_32KHZ       B10000000
enum SQWAVE_FREQS_t {
    SQWAVE_1_HZ = B10000011,
    SQWAVE_1024_HZ = B10000001,
    SQWAVE_4096_HZ = 0,
    SQWAVE_8192_HZ = 0,
    SQWAVE_NONE = B00000000
};

#define ALARM_1 1                  // constants for alarm functions
#define ALARM_2 2

#define TIME_H_DIFF 30	// 30 years difference between 2000 and 1970 when using TimeLib functions.

/* DS3232RTC compatibility Ends */

#include <Arduino.h>
#include <TimeLib.h>
#include <Wire.h>

extern TwoWire Wire;

/* arduino class */
class PCF8563 {
    public:
    PCF8563(bool initI2C = true);

    void zeroClock();  /* Zero date/time, alarm / timer, default clkout */
    void clearStatus(); /* set both status bytes to zero */
    byte readStatus2();
    void clearVoltLow(void); /* Only clearing is possible */

    void getDateTime();     /* get date and time vals to local vars */
    void setDateTime(byte day, byte weekday, byte month, bool century, byte year,
                     byte hour, byte minute, byte sec);
    void getAlarm();  // same as getDateTime
    bool alarmEnabled();  // true if alarm interrupt is enabled
    bool alarmActive();   // true if alarm is active (going off)
    void enableAlarm(); /* activate alarm flag and interrupt */
    /* set alarm vals, 99=ignore */
    void setAlarm(byte min, byte hour, byte day, byte weekday);
    void clearAlarm(); /* clear alarm flag and interrupt */
    void resetAlarm();  /* clear alarm flag but leave interrupt unchanged */

    bool timerEnabled();  // true if timer and interrupt is enabled
    bool timerActive();   // true if timer is active (going off)
    void enableTimer(void); // activate timer flag and interrupt
    void setTimer(byte value, byte frequency, bool is_pulsed);  // set value & frequency
    void clearTimer(void); // clear timer flag, and interrupt, leave value unchanged
    void resetTimer(void); // same as clearTimer() but leave interrupt unchanged */

    void setSquareWave(byte frequency);
    void clearSquareWave();

    /* Return leap-days between start (inclusive) and end (exclusive) */
    int leapDaysBetween(byte century_start, byte year_start,
                        byte century_end, byte year_end) const;
    /* Return True if century (1: 1900, 0:2000) + decade is a leap year. */
    bool isLeapYear(byte century, int year) const;
    /* Return number of days in any month of any decade of any year */
    byte daysInMonth(byte century, byte year, byte month) const;
    /* Return the number of days since the beginning of a particular year*/
    byte daysInYear(byte century, byte year, byte month, byte day) const;
    /* Return the weekday for any date after 1900 */
    byte whatWeekday(byte day, byte month, byte century, int year) const;

    bool getVoltLow();
    byte getSecond();
    byte getMinute();
    byte getHour();
    byte getDay();
    byte getMonth();
    byte getYear();
    bool getCentury();
    byte getWeekday();
    byte getStatus1();
    byte getStatus2();

    byte getAlarmMinute();
    byte getAlarmHour();
    byte getAlarmDay();
    byte getAlarmWeekday();

    byte getTimerControl();
    byte getTimerValue();

    // Sets date/time to static fixed values, disable all alarms
    // use zeroClock() above to guarantee lowest possible values instead.
    void initClock();
    // Slightly unsafe, don't use for new code, use above instead!
    void setTime(byte hour, byte minute, byte sec);
    void getTime();  // unsafe, don't use
    void setDate(byte day, byte weekday, byte month, bool century, byte year);
    void getDate();  // unsafe, don't use

	// DS3232RTC compat
	time_t get();
	byte set(time_t t);
	byte read(tmElements_t &tm);
	byte write(tmElements_t &tm);
	void setAlarm(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate);
	void setAlarm(ALARM_TYPES_t alarmType, byte minutes, byte hours, byte daydate);
	void alarmInterrupt(byte alarmNumber, bool alarmEnabled);
	bool alarm(byte alarmNumber);
	bool checkAlarm(byte alarmNumber);
	bool clearAlarm(byte alarmNumber);
	void squareWave(SQWAVE_FREQS_t freq);
	bool oscStopped(bool clearOSF = false);
	int16_t temperature();
	// DS3232RTC compat End
	static byte errCode;

    private:
    /* methods */
    byte decToBcd(byte value);
    byte bcdToDec(byte value);
    /* time variables */
    byte _hour;
    byte _minute;
    bool _volt_low;
    byte _sec;
    byte _day;
    byte _weekday;
    byte _month;
    byte _year;
    /* alarm */
    byte _alarm_hour;
    byte _alarm_minute;
    byte _alarm_weekday;
    byte _alarm_day;
    /* CLKOUT */
    byte _squareWave;
    /* timer */
    byte _timer_control;
    byte _timer_value;
    /* support */
    byte _status1;
    byte _status2;
    bool _century;

    int Rtcc_Addr;
};


inline int PCF8563::leapDaysBetween(byte century_start, byte year_start,
                                        byte century_end, byte year_end) const {
    // Credit: Victor Haydin via stackoverflow.com
    int span_start = 2000 - (century_start * 100) + year_start;
    int span_end = 2000 - (century_end * 100) + year_end - 1;  // less year_end
    // Subtract leap-years before span_start, from leap-years before span_end
    return ((span_end / 4) - (span_end / 100) + (span_end / 400)) -
           ((span_start / 4) - (span_start / 100) + (span_start / 400));
}


inline bool PCF8563::isLeapYear(byte century, int year) const
{
    year = 2000 - (century * 100) + year;
    if ((year % 4) != 0)
        return false;
    else if ((year % 100) != 0)
        return true;
    else if ((year % 400) != 0)
        return false;
    else
        return true;
}


inline byte PCF8563::daysInMonth(byte century,
                                     byte year,
                                     byte month) const
{
    const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    byte dim = days[month];
    if (month == 2 && isLeapYear(century, year))
        dim += 1;
    return dim;
}


inline byte PCF8563::daysInYear(byte century,
                                    byte year,
                                    byte month,
                                    byte day) const
{
    const int days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    byte total = days[month - 1] + day;
    if ((month > 2) and isLeapYear(century, year))
        total += 1;
    return total;
}


inline byte PCF8563::whatWeekday(byte day, byte month,
                                     byte century, int year) const
{
    year = 2000 - (century * 100) + year;
    // Credit: Tomohiko Sakamoto
    // http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
    year -= month < 3;
    static int trans[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    return (year + year / 4 - year / 100 + year / 400 +
            trans[month - 1] + day) % 7;
}
#endif