
#include "v8.h"
#include "v8_handle.h"
#include "v8_value.h"
#include "v8_string.h"

using namespace v8;

namespace {  
  VALUE StringClass;
  
  Persistent<String>& unwrap(VALUE value) {
    return rr_v8_handle<String>(value);
  }
  VALUE New(VALUE string_class, VALUE data) {
    HandleScope handles;
    VALUE str = rb_funcall(data, rb_intern("to_s"), 0);
    return rr_v8_handle_new(string_class, String::New(RSTRING_PTR(str), RSTRING_LEN(str)));
  }
  VALUE NewSymbol(VALUE string_class, VALUE data) {
    HandleScope scope;
    VALUE str = rb_funcall(data, rb_intern("to_s"), 0);
    return rr_v8_handle_new(string_class, String::NewSymbol(RSTRING_PTR(str), RSTRING_LEN(str)));
  }
  VALUE Utf8Value(VALUE self) {
    HandleScope handles;
    Handle<String> str = unwrap(self);
    return rb_str_new(*String::Utf8Value(str), str->Utf8Length());
  }
  VALUE Utf16Value(VALUE self) {
    //How are UTF16 strings represented in ruby 1.8, 1.9
    return Qnil;
  }
  VALUE AsciiValue(VALUE self) {
    HandleScope handles;
    Handle<String> str = unwrap(self);
    return rb_str_new(*String::AsciiValue(str), str->Length());
  }
}

VALUE rr_reflect_v8_string(Handle<Value> value) {
  return rr_v8_handle_new(StringClass, Handle<String>::Cast(value));
}

void rr_init_string() {
  StringClass = rr_define_class("String", rr_v8_value_class());
  rr_define_singleton_method(StringClass, "New", New, 1);
  rr_define_singleton_method(StringClass, "NewSymbol", NewSymbol, 1);
  rr_define_method(StringClass, "Utf8Value", Utf8Value, 0);
  rr_define_method(StringClass, "Utf16Value", Utf16Value, 0);
  rr_define_method(StringClass, "AsciiValue", AsciiValue, 0);
}
