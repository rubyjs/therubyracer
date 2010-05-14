#include "v8_array.h"

using namespace v8;

void rr_init_v8_array() {
  
}

VALUE rr_reflect_v8_array(Handle<Value> value) {
  Local<Array> array(Array::Cast(*value));
  VALUE rb_array = rb_ary_new2(array->Length());
  for (unsigned int i = 0; i < array->Length(); i++) {
    Local<Value> value = array->Get(Integer::New(i));
    rb_ary_push(rb_array, rr_v82rb(value));
  }
  return rb_array;
  
}