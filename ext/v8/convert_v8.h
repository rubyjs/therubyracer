#ifndef __convert_v8_h__
#define __convert_v8_h__

#include <v8.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

template<class T, class R> class V8HandleSource {

  T dest;

 public:

  V8HandleSource() {}
  ~V8HandleSource() {}

  bool operator() (v8::Handle<v8::Value>& value, R& result) {

    // a bit klunky, but alternative is to evaluate what type the
    // object is twice, which is unappealing
    
    if (value.IsEmpty()) {
      result = dest.pushNull();
      return true;
    }

    if (value->IsUndefined()) {
      result = dest.pushNull();
      return true;
    }

    if(value->IsNull()) {
      result = dest.pushNull();
      return true;
    }
    
    if(value->IsTrue()) {
      result = dest.pushBool(true);
      return true;
    }

    if(value->IsFalse()) {
      result = dest.pushBool(false);
      return true;
    }

    if(value->IsString()) {
      v8::Local<v8::String> str = value->ToString();
      result = convertString(str);
      return true;
    }

    if(value->IsInt32()) {
      result = dest.pushInt(value->Int32Value());
      return true;
    }

    if(value->IsNumber()) {
      result = dest.pushDouble(value->NumberValue());
      return true;
    }
    
    result = dest.pushNull();
    return false;
  }

  R convertString(v8::Local<v8::String>& str) {
    char buffer[1024];
    size_t total = 0;
    size_t remaining = str->Length();
    std::string output;
    while (remaining > 0) {
      size_t toCopy = remaining > sizeof(buffer) ? sizeof(buffer) : remaining;
      str->WriteAscii(buffer, total, toCopy);
      output.append(buffer, toCopy);
      total += toCopy;
      remaining -= toCopy;
    }
    return dest.pushString(output);    
  }


};

/**
 * StringDest is a data type conversion destination that converts
 * any argument into a string.  It should have all methods listed
 * in data_conversion.txt so it can be used as a template argument
 * for a conversion source class.
 */
class V8LocalDest {

public:
    V8LocalDest();
    ~V8LocalDest();

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
};


#endif
