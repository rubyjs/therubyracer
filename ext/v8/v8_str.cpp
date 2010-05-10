
#include "v8_str.h"
#include "v8.h"
#include "v8_ref.h"

using namespace v8;

void rr_init_str() {
  VALUE String = rr_define_class("String");
  RR_DEFINE_SINGLETON_METHOD(String, "new", v8_str_new, 1);
  RR_DEFINE_METHOD(String, "to_s", v8_str_to_s, 0);
}

VALUE v8_str_new(VALUE clazz, VALUE str) {
  HandleScope handles;
  return V8_Ref_Create(clazz, String::New(RSTRING_PTR(str)));
}

VALUE v8_str_to_s(VALUE self){
  HandleScope handles;
  Local<String> str = V8_Ref_Get<String>(self);
  return rb_str_new2(*String::AsciiValue(str));
}
