#include "defines.h"
#if GSR_WATCHFACES && GSR_STARFIELD

#include <math.h>

#include <inttypes.h>

#include "moonPhaser.h"

template<typename T, typename T2>
inline T map(T2 val, T2 in_min, T2 in_max, T out_min, T out_max) {
		return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static double _Julian( int32_t year, int32_t month, const double &day )
{
  int32_t b, c, e;
  b = 0;
  if (month < 3) {
    year--;
    month += 12;
  }
  if (year > 1582 || (year == 1582 && month > 10) ||
      (year == 1582 && month == 10 && day > 15)) {
    int32_t a;
    a = year / 100;
    b = 2 - a + a / 4;
  }
  c = 365.25 * year;
  e = 30.6001 * (month + 1);
  return b + c + e + day + 1720994.5;
}

static double _sun_position( const double &j )
{
  double n, x, e, l, dl, v;
  int32_t i;
  n = 360 / 365.2422 * j;
  i = n / 360;
  n = n - i * 360.0;
  x = n - 3.762863;
  if (x < 0) x += 360;
  x *= DEG_TO_RAD;
  e = x;
  do {
    dl = e - .016718 * sin(e) - x;
    e = e - dl / (1 - .016718 * cos(e));
  } while (fabs(dl) >= 1e-12);
  v = 360 / PI * atan(1.01686011182 * tan(e / 2));
  l = v + 282.596403;
  i = l / 360;
  l = l - i * 360.0;
  return l;
}

static double _moon_position( const double &j, const double &ls )
{
  double ms, l, mm, ev, sms, ae, ec;
  int32_t i;
  ms = 0.985647332099 * j - 3.762863;
  if (ms < 0) ms += 360.0;
  l = 13.176396 * j + 64.975464;
  i = l / 360;
  l = l - i * 360.0;
  if (l < 0) l += 360.0;
  mm = l - 0.1114041 * j - 349.383063;
  i = mm / 360;
  mm -= i * 360.0;
  ev = 1.2739 * sin((2 * (l - ls) - mm) * DEG_TO_RAD);
  sms = sin(ms * DEG_TO_RAD);
  ae = 0.1858 * sms;
  mm += ev - ae - 0.37 * sms;
  ec = 6.2886 * sin(mm * DEG_TO_RAD);
  l += ev + ec - ae + 0.214 * sin(2 * mm * DEG_TO_RAD);
  l = 0.6583 * sin(2 * (l - ls) * DEG_TO_RAD) + l;
  return l;
}

static moonData_t _getPhase( const int32_t &year, const int32_t &month, const int32_t &day, const double &hour )
{
/*
  Calculates the phase of the moon at the given epoch.
  returns the moon percentage that is lit as a real number (0-1)
*/
  double j = _Julian(year, month, (double)day + hour / 24.0) - 2444238.5;
  double ls = _sun_position(j);
  double lm = _moon_position(j, ls);
  double t = lm - ls;
  if (t < 0) t += 360;
  moonData_t returnValue;
  returnValue.angle = t;
  returnValue.percentLit = (1.0 - cos((lm - ls) * DEG_TO_RAD)) / 2;
  return returnValue;
}

moonData_t moonPhaser::getPhase( const int32_t &year, const int32_t &month, const int32_t &day, const double &hour )
{
  return _getPhase( year, month, day, hour );
}
#endif