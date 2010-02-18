#include "converters.h"
#include "callbacks.h"
#include "v8_ref.h"
#include "v8_obj.h"
#include "v8_cxt.h"
#include "v8_template.h"

using namespace v8;

namespace {
  std::string UNDEFINED_STR("undefined");
}

VALUE V8_To;

VALUE V82RB(Handle<Value>& value) {
  convert_v8_to_rb_t convert;
  VALUE result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsArray()) {  
    Local<Array> array(Array::Cast(*value));
    VALUE rb_array = rb_ary_new2(array->Length());
    for (unsigned int i = 0; i < array->Length(); i++) {
      Local<Value> value = array->Get(Integer::New(i));
      rb_ary_push(rb_array, V82RB(value));
    }
    return rb_array;
  }
  
  if (value->IsObject()) {
    Local<Object> object(Object::Cast(*value));
    Local<Value> peer = object->GetHiddenValue(String::New("TheRubyRacer::RubyObject"));
    if (peer.IsEmpty()) {
      VALUE context_ref = V8_Ref_Create(V8_C_Context, Context::GetCurrent());
      object->SetHiddenValue(String::New("TheRubyRacer::Context"), External::Wrap((void *)context_ref));
      return V8_Ref_Create(V8_C_Object, value, context_ref);
    } else {      
      return (VALUE)External::Unwrap(peer);
    }
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
  Local<Value> result;
  if (convert(value, result)) {
    return result;
  }
  Local<Object> o = Racer_Create_V8_ObjectTemplate(value)->NewInstance();
  o->SetHiddenValue(String::New("TheRubyRacer::RubyObject"), External::Wrap((void *) value));
  return o;
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