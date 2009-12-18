#ifndef _RUBY_V8_FUNCTION_
#define _RUBY_V8_FUNCTION_

#include "v8.h"
#include "ruby.h"
#include "v8_ref.h"

extern VALUE V8_C_Function;

VALUE V8_Wrap_Function(v8::Handle<v8::Function> f);
#endif