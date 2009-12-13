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

  R push(v8::Handle<v8::Value>& value) {

    if (value->IsUndefined()) {
      return dest.pushNull();
    }

    if(value->IsNull()) {
      return dest.pushNull();
    }
    
    if(value->IsTrue()) {
      return dest.pushBool(true);
    }

    if(value->IsFalse()) {
      return dest.pushBool(false);
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
      return dest.pushString(output);
    }

    if(value->IsInt32()) {
      return dest.pushInt(value->Int32Value());
    }

    if(value->IsNumber()) {
      return dest.pushDouble(value->NumberValue());
    }
    
    if (value->IsObject()) {
      v8::Local<v8::Object> object(v8::Object::Cast(*value));
      return dest.pushObject(object);
    }
    
    return dest.pushNull();
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

    v8::Local<v8::Value> pushString(const std::string& value) {
        return v8::Local<v8::Value>::New(v8::String::New(value.c_str()));
    }

    v8::Local<v8::Value> pushInt(int64_t value) {
        return v8::Local<v8::Value>::New(v8::Integer::New(value));
    }

    v8::Local<v8::Value> pushDouble(double value) {
        return v8::Local<v8::Value>::New(v8::Number::New(value));
    }

    v8::Local<v8::Value> pushBool(bool value) {
        return v8::Local<v8::Value>::New(v8::Boolean::New(value));
    }

    v8::Local<v8::Value> pushNull() {
      return v8::Local<v8::Value>::New(v8::Null());
    }
    
    v8::Local<v8::Value> pushUndefined() {
      return v8::Local<v8::Value>::New(v8::Undefined());
    }
    
    // v8:Local<v8:Value> pushFunction(Function) {
    //   
    // }
};


#endif
