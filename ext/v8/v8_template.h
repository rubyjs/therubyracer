#ifndef _RUBY_V8_TEMPLATE_
#define _RUBY_V8_TEMPLATE_ 

#include "rr.h"

void rr_init_template();
v8::Handle<v8::ObjectTemplate> rr_template_std_rubyobject();

//kill
VALUE v8_ObjectTemplate_New(VALUE clazz);

#endif