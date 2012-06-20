#include "rr.h"

namespace rr {

  VALUE Backref::Storage;
  ID Backref::_new;
  ID Backref::object;

  void Backref::Init() {
    Storage = rb_eval_string("Ref::WeakReference");
    rb_gc_register_address(&Storage);
    _new = rb_intern("new");
    object = rb_intern("object");
  }

  Backref::Backref(VALUE initial) {
    allocate(initial);
    this->storage = rb_funcall(Storage, _new, 1, initial);
    rb_gc_register_address(&storage);
  }

  Backref::~Backref() {
    deallocate();
  }

  void Backref::allocate(VALUE data) {
    this->storage = rb_funcall(Storage, _new, 1, data);
    rb_gc_register_address(&storage);
  }

  void Backref::deallocate() {
    rb_gc_unregister_address(&storage);
  }

  VALUE Backref::get() {
    return rb_funcall(storage, object, 0);
  }

  VALUE Backref::set(VALUE data) {
    deallocate();
    allocate(data);
    return data;
  }

  v8::Handle<v8::Value> Backref::toExternal() {
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