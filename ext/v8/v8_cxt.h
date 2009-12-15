#ifndef _RUBY_V8_CXT_
#define _RUBY_V8_CXT_ 

#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"

VALUE v8_Context_New(int argc, VALUE *argv, VALUE self);
VALUE v8_cxt_Global(VALUE self);
VALUE v8_cxt_open(VALUE self);

#endif