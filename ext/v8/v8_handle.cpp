
#include "rr.h"
#include "v8_handle.h"

using namespace v8;

v8_handle::v8_handle(Handle<void> handle) : handle(Persistent<void>::New(handle)) {
  this->references = rb_hash_new();
  this->dead = false;
}

v8_handle::~v8_handle() {
  handle.Dispose();
  handle.Clear();
  dead = true;
}

void v8_handle::set_internal(const char* name, VALUE value) {
  rb_hash_aset(this->references, rb_str_new2(name), value);
}

VALUE v8_handle::get_internal(const char* name) {
  return rb_funcall(this->references, rb_intern("[]"), 1, rb_str_new2(name));
}

namespace {
  void gc_mark(v8_handle* handle) {
    rb_gc_mark(handle->references);
  }

  void gc_free(v8_handle* handle) {
    delete handle;
  }

  VALUE New(VALUE self, VALUE handle) {
    if (RTEST(handle)) {
      Persistent<void> that = rr_v8_handle<void>(handle);
      return rr_v8_handle_new(self, that);
    } else {
      return rr_v8_handle_new(self, Handle<void>());
    }
  }

  VALUE IsEmpty(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsEmpty());
  }

  VALUE Clear(VALUE self) {
    rr_v8_handle<void>(self).Clear();
    return Qnil;
  }

  VALUE Dispose(VALUE self) {
    rr_v8_handle<void>(self).Dispose();
    return Qnil;
  }

  void RubyWeakReferenceCallback(Persistent<Value> value, void* parameter) {
    value.Dispose();
    v8_handle* handle = rr_v8_handle_raw((VALUE)parameter);
    handle->handle.Dispose();
    handle->handle.Clear();
    handle->dead = true;
    VALUE callback = rr_v8_handle_get_internal((VALUE)parameter, "weakref_callback");
    if (RTEST(callback)) {
      rb_funcall(callback, rb_intern("call"), 0);
    }
  }


  VALUE MakeWeak(VALUE self) {
    VALUE callback = rb_block_given_p() ? rb_block_proc() : Qnil;
    rr_v8_handle_set_internal(self,"weakref_callback", callback);
    rr_v8_handle<void>(self).MakeWeak((void*)self, RubyWeakReferenceCallback);
    return Qnil;
  }

  VALUE ClearWeak(VALUE self) {
    rr_v8_handle<void>(self).ClearWeak();
    return Qnil;
  }

  VALUE IsNearDeath(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsNearDeath());
  }

  VALUE IsWeak(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsWeak());
  }

  VALUE dead_p(VALUE self) {
    return rr_v8_handle_raw(self)->dead ? Qtrue : Qfalse;
  }
}

void rr_init_handle() {
  VALUE HandleClass = rr_define_class("Handle");
  rr_define_method(HandleClass, "dead?", dead_p, 0);
  rr_define_singleton_method(HandleClass, "New", New, 1);
  rr_define_method(HandleClass, "IsEmpty", IsEmpty, 0);
  rr_define_method(HandleClass, "Clear", Clear, 0);
  rr_define_method(HandleClass, "Dispose", Dispose, 0);
  rr_define_method(HandleClass, "MakeWeak", MakeWeak, 0);
  rr_define_method(HandleClass, "ClearWeak", ClearWeak, 0);
  rr_define_method(HandleClass, "IsNearDeath", IsNearDeath, 0);
  rr_define_method(HandleClass, "IsWeak", IsWeak, 0);
}

VALUE rr_v8_handle_new(VALUE klass, v8::Handle<void> handle) {
  return Data_Wrap_Struct(klass, gc_mark, gc_free, new v8_handle(handle));
}

VALUE rr_v8_handle_class() {
  return rr_define_class("Handle");
}

void rr_v8_handle_set_internal(VALUE handle, const char* name, VALUE value) {
  rr_v8_handle_raw(handle)->set_internal(name, value);
}

VALUE rr_v8_handle_get_internal(VALUE handle, const char* name) {
  return rr_v8_handle_raw(handle)->get_internal(name);
}

v8_handle* rr_v8_handle_raw(VALUE value) {
  v8_handle* handle = 0;
  Data_Get_Struct(value, struct v8_handle, handle);
  return handle;
}
