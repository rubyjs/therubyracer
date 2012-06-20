#include "rr.h"

namespace rr {

  VALUE Backref::Storage;
  ID Backref::_new;
  ID Backref::access;

  void Backref::Init() {
    Storage = rb_eval_string("require 'v8/util/weakcell'; V8::Util::Weakcell::Storage");
    rb_gc_register_address(&Storage);
    _new = rb_intern("new");
    access = rb_intern("access");
  }

  Backref::Backref() {
    this->storage = rb_funcall(Storage, _new, 0);
    rb_gc_register_address(&storage);
  }

  Backref::~Backref() {
    rb_gc_unregister_address(&storage);
  }

  VALUE Backref::get(VALUE (*populate)(ANYARGS), VALUE data) {
    VALUE args[0];
    return rb_block_call(storage, access, 0, args, populate, data);
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