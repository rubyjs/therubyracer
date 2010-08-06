#include "rr.h"
#include "v8_locker.h"

using namespace v8;

namespace {
  VALUE Lock(VALUE self) {
    Locker locker;
    return rb_yield(Qnil);
  }
  VALUE Unlock(VALUE self) {
    Unlocker unlocker;
    return rb_yield(Qnil);
  }
  VALUE StartPreemption(VALUE self, VALUE thread_id) {
    Locker::StartPreemption(NUM2INT(rb_to_int(thread_id)));
    return Qnil;
  }
  VALUE StopPreemption(VALUE self) {
    Locker::StopPreemption();
    return Qnil;
  }
  VALUE IsLocked(VALUE self) {
    return rr_v82rb(Locker::IsLocked());
  }
  VALUE IsActive(VALUE self) {
    return rr_v82rb(Locker::IsActive());
  }
}

void rr_init_v8_locker() {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  VALUE LockerModule = rb_define_module_under(V8_C, "Locker");
  rr_define_singleton_method(V8_C, "Locker", Lock, 0);
  rr_define_singleton_method(V8_C, "Unlocker", Unlock, 0);
  rr_define_singleton_method(LockerModule, "StartPreemption", StartPreemption, 1);
  rr_define_singleton_method(LockerModule, "StopPreemption", StopPreemption, 0);
  rr_define_singleton_method(LockerModule, "IsLocked", IsLocked, 0);
  rr_define_singleton_method(LockerModule, "IsActive", IsActive, 0);
}
