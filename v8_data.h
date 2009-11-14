#ifndef __v8_data_h__
#define __v8_data_h__

#include "v8.h"
#include "stdint.h"
#include <stdio.h>
#include <string>

template<class T, class R> class V8HandleSource {

  T dest;

 public:

  V8HandleSource() {}
  ~V8HandleSource() {}

  R push(v8::Handle<v8::Value>& value, const char* name = 0) {

    if (value->IsUndefined()) {
      return dest.pushNull(name);
    }

    if(value->IsNull()) {
      return dest.pushNull(name);
    }
    
    if(value->IsTrue()) {
      return dest.pushBool(true, name);
    }

    if(value->IsFalse()) {
      return dest.pushBool(false, name);
    }

    if(value->IsString()) {
      v8::Local<v8::String> str = value->ToString();
      char buffer[1024];
      int strlen = str->Length();
      std::string output(strlen, 0);
      for (int total = 0; total < strlen;) {
        int written = str->WriteAscii(buffer, total, 1024);
        output.replace(total, written, buffer);
        total += written;
      }
      return dest.pushString(output, name);
    }

    if(value->IsInt32()) {
      return dest.pushInt(value->Int32Value(), name);
    }

    if(value->IsNumber()) {
      return dest.pushDouble(value->NumberValue(), name);
    }
    
    if (value->IsObject()) {
      v8::Local<v8::Object> object(v8::Object::Cast(*value));
      return dest.pushObject(object, name);
    }
    
    return dest.pushNull(name);
  }

};

/**
 * StringDest is a data type conversion destination that converts
 * any argument into a string.  It should have all methods listed
 * in data_conversion.txt so it can be used as a template argument
 * for a conversion source class.
 */
class V8HandleDest {

public:
    V8HandleDest();
    ~V8HandleDest();

    v8::Persistent<v8::Value> pushString(const std::string& value, const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::String::New(value.c_str()));
    }

    v8::Persistent<v8::Value> pushInt(int64_t value, const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::Integer::New(value));
    }

    v8::Persistent<v8::Value> pushDouble(double value, const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::Number::New(value));
    }

    v8::Persistent<v8::Value> pushBool(bool value, const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::Boolean::New(value));
    }

    v8::Persistent<v8::Value> pushNull(const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::Integer::New(0)); // so WRONG!! not sure how to make a null from API
    }
    
    v8::Persistent<v8::Value> pushUndefined(const char* name=0) {
        return v8::Persistent<v8::Value>::New(v8::Integer::New(0)); // so WRONG!! not sure how to make a null from API
    }

};


#endif
