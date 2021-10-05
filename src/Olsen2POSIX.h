#ifndef WATCHY_OLSEN2POSIX_H
#define WATCHY_OLSEN2POSIX_H

#include <Arduino.h>

class Olsen2POSIX{
    public:
        String getPOSIX(String Olsen);
        const String TZMISSING = "--MISSING--";
};

#endif
