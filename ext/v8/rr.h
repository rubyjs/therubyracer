#ifndef _THE_RUBY_RACER_
#define _THE_RUBY_RACER_ 

#include <ruby.h>
#include <v8.h>

#define rr_define_method(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define rr_define_singleton_method(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

VALUE rr_define_class(const char *name, VALUE superclass = rb_cObject);
VALUE rr_str_to_perl_case(VALUE str);
VALUE rr_str_to_camel_case(VALUE str);

VALUE rr_to_ruby(v8::Handle<v8::Value> value);
VALUE rr_to_ruby(v8::Handle<v8::Boolean> value);
VALUE rr_to_ruby(v8::Handle<v8::Number> value);
VALUE rr_to_ruby(v8::Handle<v8::String> value);
VALUE rr_to_ruby(v8::Handle<v8::Object> value);
VALUE rr_to_ruby(v8::Handle<v8::Integer> value);
VALUE rr_to_ruby(v8::Handle<v8::Uint32> value);
VALUE rr_to_ruby(v8::Handle<v8::Int32> value);
VALUE rr_to_ruby(bool value);
VALUE rr_to_ruby(double value);
VALUE rr_to_ruby(int64_t value);
VALUE rr_to_ruby(uint32_t value);
VALUE rr_to_ruby(int32_t value);

#endif