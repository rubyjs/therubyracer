
#include "v8_str.h"
#include "v8.h"
#include "v8_ref.h"

using namespace v8;

namespace {  
  VALUE StringClass;
  
  Handle<String> unwrap(VALUE value) {
    return V8_Ref_Get<String>(value);
  }
  VALUE New(VALUE klazz, VALUE data) {
    VALUE str = rb_funcall(data, rb_intern("to_s"), 0);
    return V8_Ref_Create(StringClass, String::New(RSTRING_PTR(str), RSTRING_LEN(str)));
  }
  VALUE Utf8Value(VALUE self) {
    return rb_str_new2(*String::Utf8Value(unwrap(self)));
  }
  VALUE Utf16Value(VALUE self) {
    //How are UTF16 strings represented in ruby 1.8, 1.9
    return Qnil;
  }
  VALUE AsciiValue(VALUE self) {
    return rb_str_new2(*String::AsciiValue(unwrap(self)));
  }
}

VALUE rr_reflect_v8_string(Handle<Value> value) {
  HandleScope handles;
  Local<String> string = String::Cast(*value);
  return V8_Ref_Create(StringClass, string);
}

void rr_init_str() {
  StringClass = rr_define_class("String");
  rr_define_singleton_method(StringClass, "New", New, 1);
  rr_define_singleton_method(StringClass, "new", New, 1);
  rr_define_method(StringClass, "Utf8Value", Utf8Value, 0);
  rr_define_method(StringClass, "Utf16Value", Utf16Value, 0);
  rr_define_method(StringClass, "AsciiValue", AsciiValue, 0);
}
