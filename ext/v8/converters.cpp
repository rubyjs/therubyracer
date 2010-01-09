#include "converters.h"
#include "callbacks.h"
#include "v8_ref.h"
#include "v8_obj.h"

using namespace v8;

namespace {
  std::string UNDEFINED_STR("undefined");
}

VALUE V82RB(Handle<Value>& value) {
  convert_v8_to_rb_t convert;
  VALUE result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsObject()) {
    Local<Object> object(Object::Cast(*value));
    return V8_Ref_Create(V8_C_Object, value);
  }
  
  return Qnil;
}

Local<Value> RB2V8(VALUE value) {
  
  VALUE valueClass = rb_class_of(value);
  
  if(valueClass == rb_cProc || valueClass == rb_cMethod) {
    Local<FunctionTemplate> t = FunctionTemplate::New(RacerRubyInvocationCallback, External::Wrap((void *)value));
    return t->GetFunction();  
  }

  convert_rb_to_v8_t convert;
  return convert(value);
}

std::string RB2String(VALUE value) {
  convert_rb_to_string_t convert;
  return convert(value);
}

std::string V82String(Handle<Value>& value) {
  convert_v8_to_string_t convert;
  std::string result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsObject()) {
    Local<Object> object(Object::Cast(*value));
    Local<String> str = object->ToString();
    if(convert(value, result)) {
      return result;
    }
  }  
  
  return UNDEFINED_STR;
}