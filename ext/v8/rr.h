#ifndef _THE_RUBY_RACER_
#define _THE_RUBY_RACER_ 

#include <ruby.h>
#include <v8.h>

#define rr_define_method(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define rr_define_singleton_method(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

VALUE rr_define_class(const char *name, VALUE superclass = rb_cObject);
VALUE rr_define_module(const char *name);
VALUE rr_define_const(const char *name, VALUE value);
VALUE rr_const_get(const char *name);
VALUE rr_define_finalizer(VALUE object, void* finalizer, VALUE data);

extern "C" VALUE rb_proc_new(VALUE (*)(ANYARGS/* VALUE yieldarg[, VALUE procarg] */), VALUE);


VALUE rr_v82rb(v8::Handle<v8::Value> value);
VALUE rr_v82rb(v8::Handle<v8::Boolean> value);
VALUE rr_v82rb(v8::Handle<v8::Number> value);
VALUE rr_v82rb(v8::Handle<v8::String> value);
VALUE rr_v82rb(v8::Handle<v8::Object> value);
VALUE rr_v82rb(v8::Handle<v8::Array> value);
VALUE rr_v82rb(v8::Handle<v8::Function> value);
VALUE rr_v82rb(v8::Handle<v8::Integer> value);
VALUE rr_v82rb(v8::Handle<v8::Uint32> value);
VALUE rr_v82rb(v8::Handle<v8::Int32> value);
VALUE rr_v82rb(v8::Handle<v8::Message> value);
VALUE rr_v82rb(v8::Handle<v8::StackTrace> value);
VALUE rr_v82rb(v8::Handle<v8::StackFrame> value);

VALUE rr_v82rb(bool value);
VALUE rr_v82rb(double value);
VALUE rr_v82rb(int64_t value);
VALUE rr_v82rb(uint32_t value);
VALUE rr_v82rb(int32_t value);

v8::Handle<v8::Value> rr_rb2v8(VALUE value);

#endif
