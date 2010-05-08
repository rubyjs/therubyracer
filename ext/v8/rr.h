#ifndef _THE_RUBY_RACER_
#define _THE_RUBY_RACER_ 

#include <ruby.h>

#define RR_DEFINE_METHOD(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define RR_DEFINE_SINGLETON_METHOD(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

#endif