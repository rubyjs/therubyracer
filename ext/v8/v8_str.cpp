
#include "v8_str.h"
#include "v8.h"
#include "v8_ref.h"

using namespace v8;

VALUE v8_str_new(VALUE clazz, VALUE str) {
  HandleScope handles;
  return V8_Ref_Create(clazz, String::New(RSTRING_PTR(str)));
}

VALUE v8_str_to_s(VALUE self){
  HandleScope handles;
  Local<String> str = V8_Ref_Get<String>(self);
  return rb_str_new2(*String::AsciiValue(str));
}
