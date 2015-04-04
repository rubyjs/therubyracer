#include "rr.h"

namespace rr {

  VALUE Backref::Storage;
  ID Backref::_new;
  ID Backref::object;

  void Backref::Init() {
    Storage = rb_eval_string("V8::Weak::Ref");
    rb_gc_register_address(&Storage);
    _new = rb_intern("new");
    object = rb_intern("object");
  }

  Backref::Backref(VALUE initial) {
    set(initial);
    rb_gc_register_address(&storage);
  }

  Backref::~Backref() {
    rb_gc_unregister_address(&storage);
  }

  VALUE Backref::set(VALUE data) {
    this->storage = rb_funcall(Storage, _new, 1, data);
    return data;
  }

  VALUE Backref::get() {
    return rb_funcall(storage, object, 0);
  }

  v8::Handle<v8::External> Backref::toExternal(v8::Isolate* isolate) {
    v8::Local<v8::External> wrapper = v8::External::New(isolate, this);
    v8::Persistent<v8::External>(isolate, wrapper).SetWeak(this, &release);

    return wrapper;
  }

  void Backref::release(const v8::WeakCallbackData<v8::External, Backref>& data) {
    // The Persistent handle is disposed of automatically.

    delete data.GetParameter();
  }

}
