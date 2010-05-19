#ifndef _RUBY_V8_TEMPLATE_
#define _RUBY_V8_TEMPLATE_ 

#include "rr.h"

void rr_init_template();

v8::Local<v8::ObjectTemplate> Racer_Create_V8_ObjectTemplate(VALUE object);

VALUE v8_ObjectTemplate_New(VALUE clazz);

#endif