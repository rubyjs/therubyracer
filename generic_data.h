#ifndef __generic_data_h__
#define __generic_data_h__

#include "v8.h"
#include <string>

class StringDest {
 
 public:
  StringDest();
  ~StringDest();

  std::string pushString(const char* value, const char* name=0) {
    std::string convertedValue(value);
    return convertedValue;
  }

  /*
  const char* pushInt(int32_t value, const char* name=0) {
    char buffer[64];
    std::sprintf(buffer, "%d", value);
    convertedValue = buffer;
    return convertedValue.c_str();
  }
  */

  std::string pushInt(int64_t value, const char* name=0) {
    char buffer[64];
    std::sprintf(buffer, "%lld", value);
    std::string convertedValue(buffer);   
    return convertedValue;
  }

  std::string pushDouble(double value, const char* name=0) {
    char buffer[64];
    std::sprintf(buffer, "%g", value);
    std::string convertedValue(buffer);   
    return convertedValue;
  }

  std::string pushBool(bool value, const char* name=0) {
    std::string convertedValue(value ? "true" : "false");
    return convertedValue;
  }

  std::string pushNull(const char* name=0) {
    return "";
  }

};

#endif
