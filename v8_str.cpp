
#include "v8_str.h"
#include "v8.h"
#include "v8_ref.h"

using namespace v8;

VALUE v8_str_new(VALUE clazz, VALUE str) {
  HandleScope handles;
  v8_ref* ref = new v8_ref(String::New(RSTRING(str)->ptr));
  return Data_Wrap_Struct(clazz, v8_ref_mark , v8_ref_free, ref);
}

VALUE v8_str_to_s(VALUE self){
  HandleScope handles;
  v8_ref* ref = 0;
  Data_Get_Struct(self, struct v8_ref, ref);
  Local<String> str = (String *)*ref->handle;
  return rb_str_new2(*String::AsciiValue(str));
}
