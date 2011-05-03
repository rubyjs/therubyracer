
#include "rr.h"
#include "v8_handle.h"

using namespace v8;

v8_handle::v8_handle(Handle<void> handle) : handle(Persistent<void>::New(handle)) {
  this->weakref_callback = Qnil;
  this->weakref_callback_parameters = Qnil;
  this->dead = false;
}

v8_handle::~v8_handle() {
  handle.Dispose();
  handle.Clear();
  dead = true;
}

namespace {
  void v8_handle_mark(v8_handle* handle) {
    rb_gc_mark(handle->weakref_callback);
    rb_gc_mark(handle->weakref_callback_parameters);
  }

  void v8_handle_free(v8_handle* handle) {
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
    VALUE self = (VALUE)parameter;
    v8_handle* handle = rr_v8_handle_raw(self);
    VALUE callback = handle->weakref_callback;
    VALUE parameters = handle->weakref_callback_parameters;
    if (RTEST(callback)) {
      rb_funcall(callback, rb_intern("call"), 2, self, parameters);
    }
    value.Dispose();
    handle->handle.Dispose();
    handle->handle.Clear();
    handle->dead = true;

  }

  VALUE MakeWeak(VALUE self, VALUE parameters, VALUE callback) {
    v8_handle* handle = rr_v8_handle_raw(self);
    handle->weakref_callback = callback;
    handle->weakref_callback_parameters = parameters;
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
  rr_define_method(HandleClass, "MakeWeak", MakeWeak, 2);
  rr_define_method(HandleClass, "ClearWeak", ClearWeak, 0);
  rr_define_method(HandleClass, "IsNearDeath", IsNearDeath, 0);
  rr_define_method(HandleClass, "IsWeak", IsWeak, 0);
}

VALUE rr_v8_handle_new(VALUE klass, v8::Handle<void> handle) {
  v8_handle* new_handle = new v8_handle(handle);
  return Data_Wrap_Struct(klass, v8_handle_mark, v8_handle_free, new_handle);
}

VALUE rr_v8_handle_class() {
  return rr_define_class("Handle");
}

v8_handle* rr_v8_handle_raw(VALUE value) {
  v8_handle* handle = 0;
  Data_Get_Struct(value, struct v8_handle, handle);
  return handle;
}
