#include "v8.h"
#include "v8_weakref.h"

using namespace v8;

v8_weakref::v8_weakref(VALUE object) {
  this->v8_active = true;
  this->rb_active = true;
  this->external = Persistent<External>::New(External::New((void *)this));
  this->external.MakeWeak(this, v8_weakref_dispose);
  this->set(object);
}

void v8_weakref::set(VALUE value) {
  this->object_id = rb_obj_id(value);
  this->rb_active = true;
  VALUE data = Data_Wrap_Struct(rb_cObject, 0, 0, this);
  rr_define_finalizer(value,(void*)v8_weakref_finalize, data);
}

VALUE v8_weakref::get() {
  if (this->object_id) {
    return rb_rescue((VALUE (*)(...))v8_weakref_id2ref, this->object_id, (VALUE (*)(...))v8_weakref_nil, Qnil);
  } else {
    return Qnil;
  }
}

VALUE v8_weakref_finalize(VALUE object_id, VALUE data) {
  v8_weakref* weakref = 0;
  Data_Get_Struct(data, struct v8_weakref, weakref);
  weakref->object_id = Qnil;
  weakref->rb_active = false;
  if (!weakref->v8_active) {
    delete weakref;
  }
  return Qnil;
}

void v8_weakref_dispose(Persistent<Value> value, void* data) {
  value.Dispose();
  value.Clear();
  v8_weakref* weakref = (v8_weakref*)data;
  weakref->v8_active = false;
  if (!weakref->rb_active) {
    delete weakref;
  }
}

VALUE v8_weakref_nil(VALUE nil, VALUE exception) {
  return nil;
}

VALUE v8_weakref_objectspace() {
  return rb_const_get(rb_cObject, rb_intern("ObjectSpace"));
}

VALUE v8_weakref_id2ref(VALUE id) {
  return rb_funcall(v8_weakref_objectspace(), rb_intern("_id2ref"), 1, id);
}

