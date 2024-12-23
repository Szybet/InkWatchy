#pragma once

#include "defines.h"

#define debugLog(command) isDebug(logFunction(__FILE__, __LINE__, __func__, command))

// https://stackoverflow.com/questions/68242576/macro-which-will-not-compile-the-function-if-not-defined
// This way, we don't waste memory!
#if DEBUG
#define isDebug(command) command;
#else
#define isDebug(...) NULL
#endif

#define BOOL_STR(b) ((b) ? String("True") : String("False"))

// https://stackoverflow.com/questions/2335888/how-to-compare-strings-in-c-conditional-preprocessor-directives
#define MYSTRING_LEN(s) strlen(s)

