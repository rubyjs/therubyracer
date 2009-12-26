#include "converters.h"

#include "v8_ref.h"
#include "v8_obj.h"

namespace {
  std::string UNDEFINED_STR("undefined");
}

VALUE V82RB(v8::Handle<v8::Value>& value) {
  convert_v8_to_rb_t convert;
  VALUE result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsObject()) {
    v8::Local<v8::Object> object(v8::Object::Cast(*value));
    return V8_Ref_Create(V8_C_Object, value);
  }
  
  return Qnil;
}

v8::Local<v8::Value> RB2V8(VALUE value) {
  convert_rb_to_v8_t convert;
  return convert(value);
}

std::string RB2String(VALUE value) {
  convert_rb_to_string_t convert;
  return convert(value);
}

std::string V82String(v8::Handle<v8::Value>& value) {
  convert_v8_to_string_t convert;
  std::string result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsObject()) {
    v8::Local<v8::Object> object(v8::Object::Cast(*value));
    v8::Local<v8::String> str = object->ToString();
    if(convert(value, result)) {
      return result;
    }
  }  
  
  return UNDEFINED_STR;
}