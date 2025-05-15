#pragma once

// Include String class - needed in Arduino environment
#include <Arduino.h>

// Temperature unit definitions
#define CELSIUS 1
#define FAHRENHEIT 2

// Ensure WATCHFACE_TEMP_TYPE is defined
#ifndef WATCHFACE_TEMP_TYPE
#define WATCHFACE_TEMP_TYPE CELSIUS
#endif

// ==============================================================================
// TEMPERATURE CONVERSION FUNCTIONS
// ==============================================================================

// Convert Celsius to Fahrenheit
inline float celsiusToFahrenheit(float celsius) {
    return (celsius * 9.0f / 5.0f) + 32.0f;
}

// Convert Fahrenheit to Celsius  
inline float fahrenheitToCelsius(float fahrenheit) {
    return (fahrenheit - 32.0f) * 5.0f / 9.0f;
}

// ==============================================================================
// TEMPERATURE SUFFIXES AND FORMATTING
// ==============================================================================

#if WATCHFACE_TEMP_TYPE == CELSIUS
    #define WF_TEMP_SUFFIX "C"
    #define WF_TEMP_SUFFIX_LONG "°C"
    
    // Function to get display temperature (input is always Celsius from weather)
    inline float getDisplayTemperature(float celsius) {
        return celsius;
    }
    
#elif WATCHFACE_TEMP_TYPE == FAHRENHEIT
    #define WF_TEMP_SUFFIX "F"
    #define WF_TEMP_SUFFIX_LONG "°F"
    
    // Function to get display temperature (input is always Celsius from weather)
    inline float getDisplayTemperature(float celsius) {
        return celsiusToFahrenheit(celsius);
    }
    
#else
    // Default to Celsius
    #define WF_TEMP_SUFFIX "C"
    #define WF_TEMP_SUFFIX_LONG "°C"
    
    inline float getDisplayTemperature(float celsius) {
        return celsius;
    }
#endif

// ==============================================================================
// HELPER FUNCTIONS
// ==============================================================================

// Format temperature as string with unit
inline String formatTemperature(float celsius, bool useLongSuffix = false) {
    float displayTemp = getDisplayTemperature(celsius);
    String suffix = useLongSuffix ? WF_TEMP_SUFFIX_LONG : WF_TEMP_SUFFIX;
    return String(int(round(displayTemp))) + suffix;
}

// Format temperature as string without unit (for space-constrained displays)
inline String formatTemperatureValue(float celsius) {
    float displayTemp = getDisplayTemperature(celsius);
    return String(int(round(displayTemp)));
}