#include "v8_handle.h"
#include "v8_array.h"
#include "v8_object.h"

using namespace v8;


namespace {
  
  VALUE ArrayClass;
  
  Persistent<Array>& unwrap(VALUE self) {
    return rr_v8_handle<Array>(self);
  }
  
  VALUE New(int argc, VALUE *argv, VALUE self) {
    if (!Context::InContext()) {
      rb_raise(rb_eScriptError, "must be in a context to call Array::New()");
      return Qnil;
    }
    VALUE length;
    rb_scan_args(argc, argv, "01", &length);
    if (NIL_P(length)) {
      length = INT2FIX(0);
    }
    HandleScope scope;
    return rr_v8_handle_new(self, Array::New(NUM2INT(length)));
  }
  
  VALUE Length(VALUE self) {
    return rr_v82rb(unwrap(self)->Length());
  }
  
  VALUE CloneElementAt(VALUE self, VALUE index) {
    return rr_v82rb(unwrap(self)->CloneElementAt(NUM2UINT(index)));
  }
}

void rr_init_v8_array() {
  ArrayClass = rr_define_class("Array", rr_v8_object_class());
  rr_define_singleton_method(ArrayClass, "New", New, -1);
  rr_define_method(ArrayClass, "Length", Length, 0);
  rr_define_method(ArrayClass, "CloneElementAt", CloneElementAt, 1);
}

VALUE rr_reflect_v8_array(Handle<Value> value) {
  return rr_reflect_v8_object_as(value, ArrayClass);
}