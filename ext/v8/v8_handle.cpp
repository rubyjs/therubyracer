
#include "rr.h"
#include "v8_ref.h"

using namespace v8;

namespace {

  VALUE New(VALUE self, VALUE handle) {
    return rr_v8_ref_create(self, rr_v8_handle(handle));
  }

  VALUE IsEmpty(VALUE self) {
    return rr_v82rb(rr_v8_handle(self).IsEmpty());
  }

  VALUE Clear(VALUE self) {
    rr_v8_handle(self).Clear();
    return Qnil;
  }

  VALUE Dispose(VALUE self) {
    rr_v8_handle(self).Dispose();
    return Qnil;
  }

  void NoopWeakReferenceCallback(Persistent<Value> object, void* parameter) {}

  VALUE MakeWeak(VALUE self) {
    rr_v8_handle(self).MakeWeak(0, NoopWeakReferenceCallback);
    return Qnil;
  }

  VALUE ClearWeak(VALUE self) {
    rr_v8_handle(self).ClearWeak();
    return Qnil;
  }

  VALUE IsNearDeath(VALUE self) {
    return rr_v82rb(rr_v8_handle(self).IsNearDeath());
  }

  VALUE IsWeak(VALUE self) {
    return rr_v82rb(rr_v8_handle(self).IsWeak());
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

