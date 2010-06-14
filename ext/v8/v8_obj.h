#ifndef _RUBY_V8_OBJECT_
#define _RUBY_V8_OBJECT_ 

#include "rr.h"

extern VALUE rr_cV8_C_Object;

void rr_init_obj();
VALUE rr_reflect_v8_object(v8::Handle<v8::Value> value);

#endif
