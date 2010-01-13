#include "v8_obj.h"
#include "v8_ref.h"
#include "converters.h"

using namespace v8;

#include <cstdio>

VALUE V8_C_Object;

namespace {
  Local<Object> unwrap(VALUE robj) {
    return V8_Ref_Get<Object>(robj);
  }
}

VALUE v8_Object_New(VALUE clazz) {
  HandleScope handles;
  return V8_Ref_Create(clazz, Object::New());
}

VALUE v8_Object_Get(VALUE self, VALUE key) {
  HandleScope handles;  
  Local<Object> obj = unwrap(self);
  VALUE keystr = rb_str_to_str(key);
  Local<Value> value = obj->Get(RB2V8(keystr));
  return V82RB(value);
}

VALUE v8_Object_Set(VALUE self, VALUE key, VALUE value) {
  HandleScope handles;
  Local<Object> obj = unwrap(self);

  VALUE keystr = rb_funcall(key, rb_intern("to_s"), 0);
  obj->Set(RB2V8(keystr), RB2V8(value));
  return Qnil;
}

VALUE v8_Object_GetPropertyNames(VALUE self) {
  HandleScope handles;
  Local<Object> object = unwrap(self);  
  Local<Value> names = object->GetPropertyNames();
  return V82RB(names);
}

VALUE v8_Object_context(VALUE self) {
  HandleScope handles;
  Local<Object> object = unwrap(self);
  Local<Value> cxt = object->GetHiddenValue(String::New("TheRubyRacer::Context"));
  return cxt.IsEmpty() ? Qnil : (VALUE)External::Unwrap(cxt);
}
