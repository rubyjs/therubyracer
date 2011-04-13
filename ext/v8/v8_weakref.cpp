#include "v8.h"
#include "v8_weakref.h"

using namespace v8;

v8_weakref::v8_weakref(VALUE object) {
  this->external = Persistent<External>::New(External::New((void *)this));
  this->external.MakeWeak(this, v8_weakref_finalize);
  this->set(object);
}

void v8_weakref::set(VALUE value) {
  this->object_id = rb_obj_id(value);
}

VALUE v8_weakref::get() {
  return rb_rescue((VALUE (*)(...))v8_weakref_id2ref, this->object_id, (VALUE (*)(...))v8_weakref_nil, Qnil);
}

void v8_weakref_finalize(Persistent<Value> value, void* weakref) {
  value.Dispose();
  value.Clear();
  delete (v8_weakref*)weakref;
}

VALUE v8_weakref_nil(VALUE nil, VALUE exception) {
  return nil;
}

VALUE v8_weakref_id2ref(VALUE id) {
  return rb_funcall(rb_const_get(rb_cObject, rb_intern("ObjectSpace")), rb_intern("_id2ref"), 1, id);
}

