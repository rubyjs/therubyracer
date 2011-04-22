#include "rr.h"
#include "v8_external.h"

#include "v8_handle.h"
#include "v8_value.h"
using namespace v8;

namespace {
  VALUE ExternalClass;
  VALUE references;

  VALUE New(VALUE self, VALUE value) {
    HandleScope scope;
    return rr_v8_handle_new(self, rr_v8_external_create(value));
  }
  VALUE Unwrap(VALUE self, VALUE value) {
    HandleScope scope;
    if (rb_obj_is_kind_of(value, self)) {
      return (VALUE)External::Unwrap(rr_v8_handle<External>(self));
    } else {
      rb_raise(rb_eArgError, "cannot unwrap %s. It is not a kind of %s", RSTRING_PTR(rb_class_name(rb_class_of(value))), RSTRING_PTR(rb_class_name(self)));
      return Qnil;
    }
  }
  VALUE _Value(VALUE self) {
    HandleScope scope;
    return (VALUE)rr_v8_handle<External>(self)->Value();
  }
  void GCWeakReferenceCallback(Persistent<Value> object, void* parameter) {
    Local<External> external(External::Cast(*object));
    rb_hash_delete(references, rb_obj_id((VALUE)external->Value()));
  }
}

void rr_init_v8_external() {
  ExternalClass = rr_define_class("External", rr_v8_value_class());
  references = rb_hash_new();
  rb_define_const(ExternalClass, "OBJECTS_REFERENCED_FROM_WITHIN_V8", references);
  rr_define_singleton_method(ExternalClass, "New", New, 1);
  rr_define_singleton_method(ExternalClass, "Unwrap", Unwrap, 1);
  rr_define_method(ExternalClass, "Value", _Value, 0);
}

VALUE rr_reflect_v8_external(Handle<Value> external) {
  return rr_v8_handle_new(ExternalClass, external);
}

Handle<Value> rr_v8_external_create(VALUE value) {
  rb_hash_aset(references, rb_obj_id(value), value);
  Local<Value> external(External::New((void *)value));
  Persistent<Value> record = Persistent<Value>::New(external);
  record.MakeWeak(NULL, GCWeakReferenceCallback);
  return external;
}