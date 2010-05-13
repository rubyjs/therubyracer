#ifndef _RUBY_V8_FUNCTION_
#define _RUBY_V8_FUNCTION_

#include "rr.h"
#include "v8.h"
#include "v8_ref.h"

extern VALUE rr_cV8_C_Function;

void rr_init_func();

VALUE rr_reflect_v8_function(v8::Handle<v8::Value> value);

#endif