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
      std::string output;
      for (int total = 0; total < strlen;) {
        int written = str->WriteAscii(buffer, 0, 1024);
        output.append(buffer, written);
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
  }

};

/*
class V8ScopeDest {

  v8::Context::Scope& scope;

 public:
  V8ScopeDest(v8::Context::Scope& scope);
  ~V8ScopeDest();

  bool pushString(const char* value, const char* name=0) {
    // convert and insert
    return false;
  }

  bool pushInt(int64_t value, const char* name=0) {
    return pushDouble(value, name);
  }

  bool pushDouble(double value, const char* name=0) {
    // convert and insert
    return false;
  }

  bool pushBool(bool value, const char* name=0) {
    // convert and insert
    return false;
  }

};
*/


#endif
