#include "rr.h"

namespace rr {

  VALUE Backref::WeakRef;
  ID Backref::_new;
  ID Backref::__getobj__;
  ID Backref::__setobj__;
  ID Backref::__weakref_alive;

  void Backref::Init() {
    WeakRef = rb_eval_string("require 'weakref'; WeakRef");
    rb_gc_register_address(&WeakRef);
    _new = rb_intern("new");
    __getobj__ = rb_intern("__getobj__");
    __setobj__ = rb_intern("__setobj__");
    __weakref_alive = rb_intern("weakref_alive?");
  }

  Backref::Backref(VALUE initial) {
    this->ref = rb_funcall(WeakRef, _new, 1, initial);
    rb_gc_register_address(&ref);
  }

  Backref::~Backref() {
    rb_gc_unregister_address(&ref);
  }

  VALUE Backref::get() {
    if (alive_p()) {
      return rb_funcall(ref, __getobj__, 0);
    } else {
      return Qnil;
    }
  }

  void Backref::set(VALUE value) {
    rb_funcall(ref, __setobj__, 1, value);
  }

  bool Backref::alive_p() {
    return RTEST(rb_funcall(ref, __weakref_alive, 0));
  }

  v8::Handle<v8::Value> Backref::to_external() {
    v8::Local<v8::Value> wrapper = v8::External::Wrap(this);
    v8::Persistent<v8::Value>::New(wrapper).MakeWeak(this, &release);
    return wrapper;
  }

  void Backref::release(v8::Persistent<v8::Value> handle, void* data) {
    handle.Dispose();
    Backref* backref = (Backref*)data;
    delete backref;
  }
}