#ifndef _RUBY_V8_CXT_
#define _RUBY_V8_CXT_ 

#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"

extern VALUE V8_C_Object;
extern VALUE V8_C_Context;

void rr_init_cxt();
VALUE rr_reflect_v8_context(v8::Handle<v8::Context> value);

#endif