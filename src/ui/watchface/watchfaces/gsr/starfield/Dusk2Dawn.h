#include "minimalDefines.h"
#if GSR_WATCHFACES && GSR_STARFIELD

/*  Dusk2Dawn.h
 *  Get time of sunrise and sunset.
 *  Created by DM Kishi <dm.kishi@gmail.com> on 2017-02-01.
 *  <https://github.com/dmkishi/Dusk2Dawn>
 */

#ifndef Dusk2Dawn_h
#define Dusk2Dawn_h

  #include "Arduino.h"
  #include <math.h>

  class Dusk2Dawn {
    public:
      Dusk2Dawn(float, float, float);
      int sunrise(int, int, int, bool);
      int sunset(int, int, int, bool);
      static bool min2str(char*, int);
    private:
      float _latitude, _longitude;
      int   _timezone;
      int   sunriseSet(bool, int, int, int, bool);
      float sunriseSetUTC(bool, float, float, float);
      float equationOfTime(float);
      float meanObliquityOfEcliptic(float);
      float eccentricityEarthOrbit(float);
      float sunDeclination(float);
      float sunApparentLong(float);
      float sunTrueLong(float);
      float sunEqOfCenter(float);
      float hourAngleSunrise(float, float);
      float obliquityCorrection(float);
      float geomMeanLongSun(float);
      float geomMeanAnomalySun(float);
      float jDay(int, int, int);
      float fractionOfCentury(float);
      float radToDeg(float);
      float degToRad(float);
      static bool zeroPadTime(char*, byte);
  };

#endif
#endif