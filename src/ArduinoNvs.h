// ArduinoNvs.h

// Copyright (c) 2018 Sinai RnD
// Copyright (c) 2016-2017 TridentTD

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __ARDUINO_NVS_H__
#define __ARDUINO_NVS_H__

#include <Arduino.h>
#include <vector>

extern "C" {
#include "esp_partition.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
}

#ifndef ARDUINONVS_SILENT
#define ARDUINONVS_SILENT 0
#endif

#if ARDUINONVS_SILENT
  #define DEBUG_PRINT(...) { }
  #define DEBUG_PRINTLN(...) { }
  #define DEBUG_PRINTF(fmt, args...) { }
#else
  #define DEBUG_PRINTER Serial
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
  #define DEBUG_PRINTF(fmt, args...) { DEBUG_PRINTER.printf(fmt,## args); }
#endif

class ArduinoNvs {
public:
  ArduinoNvs();

  bool    begin(String namespaceNvs = "storage");
  void    close();

  bool    eraseAll(bool forceCommit = true);
  bool    erase(String key, bool forceCommit = true);

  bool    setInt(String key, uint8_t value, bool forceCommit = true);
  bool    setInt(String key, int16_t value, bool forceCommit = true);
  bool    setInt(String key, uint16_t value, bool forceCommit = true);
  bool    setInt(String key, int32_t value, bool forceCommit = true);
  bool    setInt(String key, uint32_t value, bool forceCommit = true);
  bool    setInt(String key, int64_t value, bool forceCommit = true);
  bool    setInt(String key, uint64_t value, bool forceCommit = true);
  bool    setFloat(String key, float value, bool forceCommit = true);
  bool    setString(String key, String value, bool forceCommit = true);
  bool    setBlob(String key, uint8_t* blob, size_t length, bool forceCommit = true);
  bool    setBlob(String key, std::vector<uint8_t>& blob, bool forceCommit = true);

  int64_t getInt(String key, int64_t default_value = 0);  // In case of error, default_value will be returned
  float   getFloat(String key, float default_value = 0);
  
  bool    getString(String key, String& res);
  String  getString(String key);

  size_t  getBlobSize(String key);  /// Returns the size of the stored blob
  bool    getBlob(String key,  uint8_t* blob, size_t length);  /// User should proivde enought memory to store the loaded blob. If length < than required size to store blob, function fails.
  bool    getBlob(String key, std::vector<uint8_t>& blob);  
  std::vector<uint8_t> getBlob(String key); /// Less eficient but more simple in usage implemetation of `getBlob()`

  bool        commit();
protected:
  nvs_handle  _nvs_handle;    
};

extern ArduinoNvs NVS;

#endif