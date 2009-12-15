#ifndef _RUBY_V8_SCRIPT_
#define _RUBY_V8_SCRIPT_ 

#include "ruby.h"

VALUE v8_script_new(VALUE self, VALUE source);
VALUE v8_script_Run(VALUE self);
#endif