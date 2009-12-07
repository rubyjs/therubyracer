#ifndef _RUBY_V8_CXT_
#define _RUBY_V8_CXT_ 

#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"

//Context::Global();
VALUE v8_cxt_Global(VALUE self);
VALUE v8_cxt_open(VALUE self);

//memory management
VALUE v8_cxt_allocate(VALUE clazz);
#endif