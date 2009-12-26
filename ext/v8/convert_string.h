#ifndef __convert_string_h__
#define __convert_string_h__

#include "v8.h"
#include <string>

/**
 * StringDest is a data type conversion destination that converts
 * any argument into a string.  It should have all methods listed
 * in data_conversion.txt so it can be used as a template argument
 * for a conversion source class.
 */
class StringDest {

public:
    StringDest();
    ~StringDest();

    std::string pushString(const std::string& value) {
        return value;
    }

#ifdef FIGURED_OUT_INT_ISSUES
    const char* pushInt(int32_t value) {
        char buffer[64];
        std::sprintf(buffer, "%d", value);
        convertedValue = buffer;
        return convertedValue.c_str();
    }
#endif

    std::string pushInt(int64_t value) {
        char buffer[64];
        std::sprintf(buffer, "%lld", value);
        std::string convertedValue(buffer);   
        return convertedValue;
    }

    std::string pushDouble(double value) {
        char buffer[64];
        std::sprintf(buffer, "%g", value);
        std::string convertedValue(buffer);   
        return convertedValue;
    }

    std::string pushBool(bool value) {
        std::string convertedValue(value ? TRUE : FALSE);
        return convertedValue;
    }

    std::string pushNull() {
      printf("I bet we aren't even getting here<br/>");
        return ""; // this kind of sucks
    }
    
    std::string pushUndefined() {
        return "undefined"; // this too
    }

    std::string pushObject(v8::Local<v8::Object>& foo) {
      return "";
    }

private:

    /**
     * constant "true" used for pushBool
     */
    static const std::string TRUE;

    /**
     * constant "false" used for pushBool
     */
    static const std::string FALSE;
};

#endif
