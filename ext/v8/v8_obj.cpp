#include "v8_obj.h"
#include "v8_ref.h"
#include "converters.h"

using namespace v8;

VALUE V8_C_Object;

VALUE v8_Object_New(VALUE clazz) {
  HandleScope handles;
  return V8_Ref_Create(clazz, Object::New());
}

VALUE v8_Object_Get(VALUE self, VALUE key) {
  HandleScope handles;  
  convert_rb_to_v8_t rb2v8;
  convert_v8_to_rb_t v82rb;
  Local<Object> obj = V8_Ref_Get<Object>(self);
  VALUE keystr = rb_funcall(key,rb_intern("to_s"), 0);
  Local<Value> value = obj->Get(rb2v8(keystr));
  return v82rb(value);
}

VALUE v8_Object_Set(VALUE self, VALUE key, VALUE value) {
  HandleScope handles;
  convert_rb_to_v8_t rb2v8;
  Local<Object> obj = V8_Ref_Get<Object>(self);
  VALUE keystr = rb_funcall(key, rb_intern("to_s"), 0);
  obj->Set(rb2v8(keystr), rb2v8(value));
  return Qnil;
}
