#include "defines.h"
#if GSR_WATCHFACES && GSR_STARFIELD
/*
  MoonPhase.h - Library to get moon phase angle
  and percentage illuminated. (as seen from Earth)
  Created by Marcel Timmer, April 28, 2018.
  Code adapted from http://www.voidware.com/phase.c
  A big thanks to Hugh at voidware for granting permission.
  Released under MIT license.
*/
#ifndef MoonPhaser_h
#define MoonPhaser_h

#include "Arduino.h"

struct moonData_t
{
  int32_t angle;
  double  percentLit;
};

class moonPhaser
{
public:
  moonData_t getPhase( const int32_t &year, const int32_t &month, const int32_t &day, const double &hour );
};
#endif
#endif