#ifndef _RUBY_V8_CXT_
#define _RUBY_V8_CXT_ 

#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"

extern VALUE rb_cV8;
extern VALUE V8_C_Object;
extern VALUE V8_C_Context;

VALUE v8_Context_New(int argc, VALUE *argv, VALUE self);
VALUE v8_Context_InContext(VALUE self);
VALUE v8_Context_GetCurrent(VALUE self);
VALUE v8_cxt_Global(VALUE self);
VALUE v8_cxt_open(VALUE self);
VALUE v8_cxt_eval(VALUE self, VALUE source, VALUE filename);
VALUE v8_cxt_eql(VALUE self, VALUE other);

#endif