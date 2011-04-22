#ifndef _RUBY_V8_OBJECT_
#define _RUBY_V8_OBJECT_ 

#include "rr.h"

void rr_init_object();
VALUE rr_v8_object_class();
VALUE rr_reflect_v8_object(v8::Handle<v8::Value> value);
VALUE rr_reflect_v8_object_as(v8::Handle<v8::Value> object, VALUE ruby_class);
#endif
