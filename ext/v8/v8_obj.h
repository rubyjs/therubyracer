#ifndef _RUBY_V8_OBJECT_
#define _RUBY_V8_OBJECT_ 

#include "rr.h"

extern VALUE rr_cV8_C_Object;

void rr_init_obj();
VALUE rr_reflect_v8_object(v8::Handle<v8::Value> value);
v8::Handle<v8::Value> rr_reflect_rb_object(VALUE value);

VALUE v8_Object_New(VALUE clazz);
VALUE v8_Object_Get(VALUE self, VALUE key);
VALUE v8_Object_Set(VALUE self, VALUE key, VALUE value);
VALUE v8_Object_GetPropertyNames(VALUE self);
VALUE v8_Object_context(VALUE self);
VALUE v8_Object_ToString(VALUE self);
#endif