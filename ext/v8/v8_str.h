#ifndef _RUBY_V8_STR_
#define _RUBY_V8_STR_

#include "ruby.h" 

VALUE v8_str_new(VALUE clazz, VALUE str);
VALUE v8_str_to_s(VALUE self);

#endif