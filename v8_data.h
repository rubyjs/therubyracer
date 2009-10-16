#ifndef __v8_data_h__
#define __v8_data_h__

#include "v8.h"
#include "stdint.h"
#include <stdio.h>


template<class T, class R> class V8HandleSource {

  T dest;

 public:

  V8HandleSource() {}
  ~V8HandleSource() {}

  R push(v8::Handle<v8::Value>& value, const char* name = 0) {

    printf("*******   enter push()\n");

    if(value->IsNull()) {
      return dest.pushNull(name);
    }
    printf("*******   isNull() == false\n");

    if(value->IsTrue()) {
      return dest.pushBool(true, name);
    }
    printf("*******   isTrue() == false\n");

    if(value->IsFalse()) {
      return dest.pushBool(false, name);
    }
    printf("*******   isFalse() == false\n");


    if(value->IsString()) {
      //v8::Local<v8:String::AsciiValue> strValue(value->ToString());
      return dest.pushString("", name);
    }
    printf("*******   isString() == false\n");

    if(value->IsInt32()) {
      return dest.pushInt(value->Int32Value(), name);
    }
    printf("*******   isInt32() == false\n");


    if(value->IsNumber()) {
      return dest.pushDouble(value->NumberValue(), name);
    }
    printf("*******   isNumber() == false\n");


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
