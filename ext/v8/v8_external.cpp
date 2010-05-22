#include "v8_external.h"
#include "rr.h"
#include "v8_ref.h"
using namespace v8;

namespace {
  
  VALUE _Value(VALUE self) {
    HandleScope scope;
    return (VALUE)V8_Ref_Get<External>(self)->Value();
  }
  VALUE Wrap(VALUE rbclass, VALUE value) {
    HandleScope scope;
    return rr_v8_ref_create(rbclass, External::Wrap((void *)value));
  }
  VALUE Unwrap(VALUE self, VALUE value) {
    if (rb_obj_is_kind_of(value, self)) {
      return _Value(value);
    } else {
      rb_raise(rb_eArgError, "cannot unwrap %s. It is not a kind of %s", RSTRING_PTR(rb_class_name(rb_class_of(value))), RSTRING_PTR(rb_class_name(self)));
      return Qnil;
    }
  }
}

void rr_init_v8_external() {
  VALUE ExternalClass = rr_define_class("External");
  rr_define_singleton_method(ExternalClass, "Wrap", Wrap, 1);
  rr_define_singleton_method(ExternalClass, "Unwrap", Unwrap, 1);
  rr_define_method(ExternalClass, "Value", _Value, 0);
}