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
 *    22/10/2014 Header/Code rearranging, alarm/timer flag masking
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

#include <Arduino.h>
#include "PCF8563.h"

PCF8563::PCF8563(bool initI2C)
{
    Wire.begin();
    Rtcc_Addr = RTCC_R>>1;
}

/* Private internal functions, but useful to look at if you need a similar func. */
byte PCF8563::decToBcd(byte val)
{
    return ( (val/10*16) + (val%10) );
}

byte PCF8563::bcdToDec(byte val)
{
    return ( (val/16*10) + (val%16) );
}

void PCF8563::zeroClock()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);        // start address

    Wire.write((byte)0x0);     //control/status1
    Wire.write((byte)0x0);     //control/status2
    Wire.write((byte)0x00);    //set seconds to 0 & VL to 0
    Wire.write((byte)0x00);    //set minutes to 0
    Wire.write((byte)0x00);    //set hour to 0
    Wire.write((byte)0x01);    //set day to 1
    Wire.write((byte)0x00);    //set weekday to 0
    Wire.write((byte)0x81);    //set month to 1, century to 1900
    Wire.write((byte)0x00);    //set year to 0
    Wire.write((byte)0x80);    //minute alarm value reset to 00
    Wire.write((byte)0x80);    //hour alarm value reset to 00
    Wire.write((byte)0x80);    //day alarm value reset to 00
    Wire.write((byte)0x80);    //weekday alarm value reset to 00
    Wire.write((byte)SQW_32KHZ); //set SQW to default, see: setSquareWave
    Wire.write((byte)0x0);     //timer off
    Wire.endTransmission();
}

void PCF8563::clearStatus()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);
    Wire.write((byte)0x0);                 //control/status1
    Wire.write((byte)0x0);                 //control/status2
    Wire.endTransmission();
}

/*
* Read status byte
*/
byte PCF8563::readStatus2()
{
    getDateTime();
    return getStatus2();
}

void PCF8563::clearVoltLow(void)
{
    getDateTime();
    // Only clearing is possible on device (I tried)
    setDateTime(getDay(), getWeekday(), getMonth(),
                getCentury(), getYear(), getHour(),
                getMinute(), getSecond());
}

/*
* Atomicly read all device registers in one operation
*/
void PCF8563::getDateTime(void)
{
    /* Start at beginning, read entire memory in one go */
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT1_ADDR);
    Wire.endTransmission();

    /* As per data sheet, have to read everything all in one operation */
    uint8_t readBuffer[16] = {0};
    Wire.requestFrom(Rtcc_Addr, 16);
    for (uint8_t i=0; i < 16; i++)
        readBuffer[i] = Wire.read();

    // status bytes
    _status1 = readBuffer[0];
    _status2 = readBuffer[1];

    // time bytes
    //0x7f = 0b01111111
    _volt_low = readBuffer[2] & RTCC_VLSEC_MASK;  //VL_Seconds
    _sec = bcdToDec(readBuffer[2] & ~RTCC_VLSEC_MASK);
    _minute = bcdToDec(readBuffer[3] & 0x7f);
    //0x3f = 0b00111111
    _hour = bcdToDec(readBuffer[4] & 0x3f);

    // date bytes
    //0x3f = 0b00111111
    _day = bcdToDec(readBuffer[5] & 0x3f);
    //0x07 = 0b00000111
    _weekday = bcdToDec(readBuffer[6] & 0x07);
    //get raw month data byte and set month and century with it.
    _month = readBuffer[7];
    if (_month & RTCC_CENTURY_MASK)
        _century = true;
    else
        _century = false;
    //0x1f = 0b00011111
    _month = _month & 0x1f;
    _month = bcdToDec(_month);
    _year = bcdToDec(readBuffer[8]);

    // alarm bytes
    _alarm_minute = readBuffer[9];
    if(B10000000 & _alarm_minute)
        _alarm_minute = RTCC_NO_ALARM;
    else
        _alarm_minute = bcdToDec(_alarm_minute & B01111111);
    _alarm_hour = readBuffer[10];
    if(B10000000 & _alarm_hour)
        _alarm_hour = RTCC_NO_ALARM;
    else
        _alarm_hour = bcdToDec(_alarm_hour & B00111111);
    _alarm_day = readBuffer[11];
    if(B10000000 & _alarm_day)
        _alarm_day = RTCC_NO_ALARM;
    else
        _alarm_day = bcdToDec(_alarm_day & B00111111);
    _alarm_weekday = readBuffer[12];
    if(B10000000 & _alarm_weekday)
        _alarm_weekday = RTCC_NO_ALARM;
    else
        _alarm_weekday = bcdToDec(_alarm_weekday & B00000111);

    // CLKOUT_control 0x03 = 0b00000011
    _squareWave = readBuffer[13] & 0x03;

    // timer bytes
    _timer_control = readBuffer[14] & 0x03;
    _timer_value = readBuffer[15];  // current value != set value when running
}


void PCF8563::setDateTime(byte day, byte weekday, byte month,
                              bool century, byte year, byte hour,
                              byte minute, byte sec)
{
    /* year val is 00 to 99, xx
        with the highest bit of month = century
        0=20xx
        1=19xx
        */
    month = decToBcd(month);
    if (century)
        month |= RTCC_CENTURY_MASK;
    else
        month &= ~RTCC_CENTURY_MASK;

    /* As per data sheet, have to set everything all in one operation */
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write(RTCC_SEC_ADDR);       // send addr low byte, req'd
    Wire.write(decToBcd(sec) &~RTCC_VLSEC_MASK); //set sec, clear VL bit
    Wire.write(decToBcd(minute));    //set minutes
    Wire.write(decToBcd(hour));        //set hour
    Wire.write(decToBcd(day));            //set day
    Wire.write(decToBcd(weekday));    //set weekday
    Wire.write(month);                 //set month, century to 1
    Wire.write(decToBcd(year));        //set year to 99
    Wire.endTransmission();
    // Keep values in-sync with device
    getDateTime();
}

/**
* Get alarm, set values to RTCC_NO_ALARM (99) if alarm flag is not set
*/
void PCF8563::getAlarm()
{
    getDateTime();
}

/*
* Returns true if AIE is on
*
*/
bool PCF8563::alarmEnabled()
{
    return getStatus2() & RTCC_ALARM_AIE;
}

/*
* Returns true if AF is on
*
*/
bool PCF8563::alarmActive()
{
    return getStatus2() & RTCC_ALARM_AF;
}

/* enable alarm interrupt
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void PCF8563::enableAlarm()
{
    getDateTime();  // operate on current values
    //set status2 AF val to zero
    _status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_TIMER_TF;
    //enable the interrupt
    _status2 |= RTCC_ALARM_AIE;

    //enable the interrupt
    Wire.beginTransmission(Rtcc_Addr);  // Issue I2C start signal
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();
}

/* set the alarm values
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void PCF8563::setAlarm(byte min, byte hour, byte day, byte weekday)
{
    getDateTime();  // operate on current values
    if (min <99) {
        min = constrain(min, 0, 59);
        min = decToBcd(min);
        min &= ~RTCC_ALARM;
    } else {
        min = RTCC_ALARM;
    }

    if (hour <99) {
        hour = constrain(hour, 0, 23);
        hour = decToBcd(hour);
        hour &= ~RTCC_ALARM;
    } else {
        hour = RTCC_ALARM;
    }

    if (day <99) {
        day = constrain(day, 1, 31);
        day = decToBcd(day); day &= ~RTCC_ALARM;
    } else {
        day = RTCC_ALARM;
    }

    if (weekday <99) {
        weekday = constrain(weekday, 0, 6);
        weekday = decToBcd(weekday);
        weekday &= ~RTCC_ALARM;
    } else {
        weekday = RTCC_ALARM;
    }

    _alarm_hour = hour;
    _alarm_minute = min;
    _alarm_weekday = weekday;
    _alarm_day = day;

    // First set alarm values, then enable
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)RTCC_ALRM_MIN_ADDR);
    Wire.write((byte)_alarm_minute);
    Wire.write((byte)_alarm_hour);
    Wire.write((byte)_alarm_day);
    Wire.write((byte)_alarm_weekday);
    Wire.endTransmission();

    PCF8563::enableAlarm();
}

void PCF8563::clearAlarm()
{
    //set status2 AF val to zero to reset alarm
    _status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_TIMER_TF;
    //turn off the interrupt
    _status2 &= ~RTCC_ALARM_AIE;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();
}

/**
* Reset the alarm leaving interrupt unchanged
*/
void PCF8563::resetAlarm()
{
    //set status2 AF val to zero to reset alarm
    _status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_TIMER_TF;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();
}

// true if timer interrupt and control is enabled
bool PCF8563::timerEnabled()
{
    if (getStatus2() & RTCC_TIMER_TIE)
        if (_timer_control & RTCC_TIMER_TE)
            return true;
    return false;
}


// true if timer is active
bool PCF8563::timerActive()
{
    return getStatus2() & RTCC_TIMER_TF;
}


// enable timer and interrupt
void PCF8563::enableTimer(void)
{
    getDateTime();
    //set TE to 1
    _timer_control |= RTCC_TIMER_TE;
    //set status2 TF val to zero
    _status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_ALARM_AF;
    //enable the interrupt
    _status2 |= RTCC_TIMER_TIE;

    // Enable interrupt first, then enable timer
    Wire.beginTransmission(Rtcc_Addr);  // Issue I2C start signal
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)_timer_control);  // Timer starts ticking now!
    Wire.endTransmission();
}


// set count-down value and frequency
void PCF8563::setTimer(byte value, byte frequency, bool is_pulsed)
{
    getDateTime();
    if (is_pulsed)
        _status2 |= is_pulsed << 4;
    else
        _status2 &= ~(is_pulsed << 4);
    _timer_value = value;
    // TE set to 1 in enableTimer(), leave 0 for now
    _timer_control |= (frequency & RTCC_TIMER_TD10); // use only last 2 bits

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)_timer_control);
    Wire.write((byte)_timer_value);
    Wire.endTransmission();

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();

    enableTimer();
}


// clear timer flag and interrupt
void PCF8563::clearTimer(void)
{
    getDateTime();
    //set status2 TF val to zero
    _status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_ALARM_AF;
    //turn off the interrupt
    _status2 &= ~RTCC_TIMER_TIE;
    //turn off the timer
    _timer_control = 0;

    // Stop timer first
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)_timer_control);
    Wire.endTransmission();

    // clear flag and interrupt
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();
}


// clear timer flag but leave interrupt unchanged */
void PCF8563::resetTimer(void)
{
    getDateTime();
    //set status2 TF val to zero to reset timer
    _status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    _status2 |= RTCC_ALARM_AF;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)_status2);
    Wire.endTransmission();
}

/**
* Set the square wave pin output
*/
void PCF8563::setSquareWave(byte frequency)
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)RTCC_SQW_ADDR);
    Wire.write((byte)frequency);
    Wire.endTransmission();
}

void PCF8563::clearSquareWave()
{
    PCF8563::squareWave(SQWAVE_NONE);
}

void PCF8563::initClock()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);        // start address

    Wire.write((byte)0x0);     //control/status1
    Wire.write((byte)0x0);     //control/status2
    Wire.write((byte)0x81);     //set seconds & VL
    Wire.write((byte)0x01);    //set minutes
    Wire.write((byte)0x01);    //set hour
    Wire.write((byte)0x01);    //set day
    Wire.write((byte)0x01);    //set weekday
    Wire.write((byte)0x01);     //set month, century to 1
    Wire.write((byte)0x01);    //set year to 99
    Wire.write((byte)0x80);    //minute alarm value reset to 00
    Wire.write((byte)0x80);    //hour alarm value reset to 00
    Wire.write((byte)0x80);    //day alarm value reset to 00
    Wire.write((byte)0x80);    //weekday alarm value reset to 00
    Wire.write((byte)0x0);     //set SQW, see: setSquareWave
    Wire.write((byte)0x0);     //timer off
    Wire.endTransmission();
}

void PCF8563::setTime(byte hour, byte minute, byte sec)
{
    getDateTime();
    setDateTime(getDay(), getWeekday(), getMonth(),
                getCentury(), getYear(), hour, minute, sec);
}

void PCF8563::setDate(byte day, byte weekday, byte month, bool century, byte year)
{
    getDateTime();
    setDateTime(day, weekday, month, century, year,
                getHour(), getMinute(), getSecond());
}

void PCF8563::getDate()
{
    getDateTime();
}

void PCF8563::getTime()
{
    getDateTime();
}

bool PCF8563::getVoltLow(void)
{
    return _volt_low;
}

byte PCF8563::getSecond() {
    return _sec;
}

byte PCF8563::getMinute() {
    return _minute;
}

byte PCF8563::getHour() {
    return _hour;
}

byte PCF8563::getAlarmMinute() {
    return _alarm_minute;
}

byte PCF8563::getAlarmHour() {
    return _alarm_hour;
}

byte PCF8563::getAlarmDay() {
    return _alarm_day;
}

byte PCF8563::getAlarmWeekday() {
    return _alarm_weekday;
}

byte PCF8563::getTimerControl() {
    return _timer_control;
}

byte PCF8563::getTimerValue() {
    // Impossible to freeze this value, it could
    // be changing during read.  Multiple reads
    // required to check for consistency.
    uint8_t last_value;
    do {
        last_value = _timer_value;
        getDateTime();
    } while (_timer_value != last_value);
    return _timer_value;
}

byte PCF8563::getDay() {
    return _day;
}

byte PCF8563::getMonth() {
    return _month;
}

byte PCF8563::getYear() {
    return _year;
}

bool PCF8563::getCentury() {
    return _century;
}

byte PCF8563::getWeekday() {
    return _weekday;
}

byte PCF8563::getStatus1() {
    return _status1;
}

byte PCF8563::getStatus2() {
    return _status2;
}

// Functions below "Mimic" the DS3232RTC code, so this library can be a drop in replacement for most Watch faces.

  // Read the current time from the RTC and return it as a time_t
// value. Returns a zero value if an I2C error occurred (e.g. RTC
// not present).
time_t PCF8563::get()
{
    tmElements_t tm;

    if ( read(tm) ) return 0;
    return( makeTime(tm) );
}

// Set the RTC to the given time_t value and clear the
// oscillator stop flag (OSF) in the Control/Status register.
// Returns the I2C status (zero if successful).
byte PCF8563::set(time_t t)
{
    tmElements_t tm;

    breakTime(t, tm);
    return ( write(tm) );
}

// Read the current time from the RTC and return it in a tmElements_t
// structure. Returns the I2C status (zero if successful).
byte PCF8563::read(tmElements_t &tm)
{
	getDateTime();
	tm.Year = _year;
	tm.Month = _month;
	tm.Day = _day;
	tm.Wday = _weekday;
	tm.Hour = _hour;
	tm.Minute = _minute;
	tm.Second = _sec;
	tm.Year += TIME_H_DIFF;	// Add the extra 30 years on when using this function.
//    tm.Year = y2kYearToTm(T.Year);
    return 0;
}

// Set the RTC time from a tmElements_t structure and clear the
// oscillator stop flag (OSF) in the Control/Status register.
// Returns the I2C status (zero if successful).
byte PCF8563::write(tmElements_t &tm)
{
	setDateTime(tm.Day, tm.Wday, tm.Month, false, tm.Year - TIME_H_DIFF, tm.Hour, tm.Minute, tm.Second);
    return 0;
}

void PCF8563::setAlarm(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate) { setAlarm(minutes, hours, daydate, 0); }
void PCF8563::setAlarm(ALARM_TYPES_t alarmType, byte minutes, byte hours, byte daydate) { setAlarm(minutes, hours, daydate, 0); }
void PCF8563::alarmInterrupt(byte alarmNumber, bool alarmEnabled) { if (alarmEnabled) enableAlarm(); else resetAlarm(); }
bool PCF8563::alarm(byte alarmNumber)
{
	if (alarmNumber == ALARM_2)
	{
		getDateTime();
		clearAlarm();
		_alarm_hour = RTCC_ALARM;
		_alarm_minute = (_minute > 58 ? 0 : _minute + 1);
		_alarm_weekday = RTCC_ALARM;
		_alarm_day = RTCC_ALARM;

		// First set alarm values, then enable
		Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
		Wire.write((byte)RTCC_ALRM_MIN_ADDR);
		Wire.write((byte)_alarm_minute);
		Wire.write((byte)_alarm_hour);
		Wire.write((byte)_alarm_day);
		Wire.write((byte)_alarm_weekday);
		Wire.endTransmission();
		enableAlarm();
	}
	return (_status2 & RTCC_ALARM_AF);
}
bool PCF8563::checkAlarm(byte alarmNumber) { return alarm(alarmNumber); }
bool PCF8563::clearAlarm(byte alarmNumber) { clearAlarm(); return true; }
void PCF8563::squareWave(SQWAVE_FREQS_t freq) { setSquareWave((byte)freq); }
bool PCF8563::oscStopped(bool clearOSF) { return false;	} // Not sure this works.
int16_t PCF8563::temperature() { return 32767; }	// 0x7FFF returns to prove it is the PCF8563 not the DS3232.
