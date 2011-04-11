
#include "rr.h"
#include "v8_handle.h"

using namespace v8;

v8_handle::v8_handle(Handle<void> handle) : handle(Persistent<void>::New(handle)) {}

v8_handle::~v8_handle() {
  handle.Dispose();
  handle.Clear();
}

namespace {
  void gc_mark(v8_handle* handle) {}

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

  void NoopWeakReferenceCallback(Persistent<Value> object, void* parameter) {}

  VALUE MakeWeak(VALUE self) {
    rr_v8_handle<void>(self).MakeWeak(0, NoopWeakReferenceCallback);
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
}

void rr_init_handle() {
  VALUE HandleClass = rr_define_class("Handle");
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

