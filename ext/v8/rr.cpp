#include "rr.h"
#include "v8_value.h"

using namespace v8;

VALUE rr_define_class(const char *name, VALUE superclass) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  return rb_define_class_under(V8_C, name, superclass);
}

VALUE rr_str_to_perl_case(VALUE str) {
  VALUE V8 = rb_define_module("V8");
  VALUE to = rb_define_module_under(V8, "To");
  return rb_funcall(to, rb_intern("perl_case"), 1, str);  
}

VALUE rr_str_to_camel_case(VALUE str) {
  VALUE V8 = rb_define_module("V8");
  VALUE to = rb_define_module_under(V8, "To");
  return rb_funcall(to, rb_intern("camel_case"), 1, str);
}

//not exported
VALUE rr_thunk(VALUE value) {
  VALUE V8 = rb_define_module("V8");
  VALUE To = rb_define_module_under(V8, "To");  
  return rb_funcall(To, rb_intern("rb"), 1, value);
}

VALUE rr_to_ruby(Handle<Value> value) {
  return rr_thunk(rr_wrap_v8_value(value));
}
VALUE rr_to_ruby(Handle<Boolean> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<Number> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<String> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<Object> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<Integer> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<Uint32> value) {
  return rr_to_ruby((Handle<Value>)value);
}
VALUE rr_to_ruby(Handle<Int32> value) {
  return rr_to_ruby((Handle<Value>)value);
}

VALUE rr_to_ruby(bool value) {
  return value ? Qtrue : Qfalse;
}

// VALUE rr_to_ruby(v8::ScriptData *data) {
//   return rr_thunk(rr_wrap_script_data(data));
// }



