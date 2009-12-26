#include "v8_obj.h"
#include "v8_ref.h"
#include "converters.h"

using namespace v8;

#include <cstdio>

VALUE V8_C_Object;

VALUE v8_Object_New(VALUE clazz) {
  HandleScope handles;
  return V8_Ref_Create(clazz, Object::New());
}

VALUE v8_Object_Get(VALUE self, VALUE key) {
  HandleScope handles;  
  Local<Object> obj = V8_Ref_Get<Object>(self);
  VALUE keystr = rb_funcall(key,rb_intern("to_s"), 0);
  Local<Value> value = obj->Get(RB2V8(keystr));
  return V82RB(value);
}

VALUE v8_Object_Set(VALUE self, VALUE key, VALUE value) {
  HandleScope handles;
  Local<Object> obj = V8_Ref_Get<Object>(self);
  VALUE keystr = rb_funcall(key, rb_intern("to_s"), 0);
  
  VALUE valueClass = rb_class_of(value);
  if(valueClass == rb_cProc) {
    //printf("** This is a proc! We should do something different.\n");
  }
  else if(valueClass == rb_cMethod) {
    //printf("** This is a method! We should do something different.\n");
  }
  
  obj->Set(RB2V8(keystr), RB2V8(value));
  return Qnil;
}
