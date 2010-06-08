#include "v8_array.h"
#include "v8_ref.h"
#include "v8_obj.h"

using namespace v8;


namespace {
  
  VALUE ArrayClass;
  
  Local<Array> unwrap(VALUE self) {
    return V8_Ref_Get<Array>(self);
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
    return rr_v8_ref_create(self, Array::New(NUM2INT(length)));
  }
  
  VALUE Length(VALUE self) {
    return rr_v82rb(unwrap(self)->Length());
  }
  
  VALUE CloneElementAt(VALUE self, VALUE index) {
    return rr_v82rb(unwrap(self)->CloneElementAt(NUM2UINT(index)));
  }
}

void rr_init_v8_array() {
  ArrayClass = rr_define_class("Array", rr_cV8_C_Object);
  rr_define_singleton_method(ArrayClass, "New", New, -1);
  rr_define_method(ArrayClass, "Length", Length, 0);
  rr_define_method(ArrayClass, "CloneElementAt", CloneElementAt, 1);
}

VALUE rr_reflect_v8_array(Handle<Value> value) {
  Local<Array> array(Array::Cast(*value));
  Local<Value> peer = array->GetHiddenValue(String::NewSymbol("TheRubyRacer::RubyObject"));
  return peer.IsEmpty() ? rr_v8_ref_create(ArrayClass, value) : (VALUE)External::Unwrap(peer);
}