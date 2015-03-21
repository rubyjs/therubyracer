#include "rr.h"

namespace rr {

  void Locker::Init() {
    ClassBuilder("Locker").
      defineSingletonMethod("IsLocked", &IsLocked).
      defineSingletonMethod("IsActive", &IsActive);

      VALUE v8 = rb_define_module("V8");
      VALUE c = rb_define_module_under(v8, "C");

      rb_define_singleton_method(c, "Locker", (VALUE (*)(...))&Lock, -1);
      rb_define_singleton_method(c, "Unlocker",(VALUE (*)(...))&Unlock, -1);
  }

  VALUE Locker::IsLocked(VALUE self, VALUE isolate) {
    return Bool(v8::Locker::IsLocked(Isolate(isolate)));
  }

  VALUE Locker::IsActive(VALUE self) {
    return Bool(v8::Locker::IsActive());
  }

  VALUE Locker::Lock(int argc, VALUE* argv, VALUE self) {
    if (!rb_block_given_p()) {
      return Qnil;
    }

    int state = 0;

    VALUE isolate, block;
    rb_scan_args(argc, argv, "10&", &isolate, &block);

    VALUE result = setupLockAndCall(Isolate(isolate), &state, block);

    if (state != 0) {
      rb_jump_tag(state);
    }

    return result;
  }

  VALUE Locker::setupLockAndCall(Isolate isolate, int* state, VALUE block) {
    v8::Locker locker(isolate);

    return rb_protect(&doLockCall, block, state);
  }

  VALUE Locker::doLockCall(VALUE block) {
    return rb_funcall(block, rb_intern("call"), 0);
  }

  VALUE Locker::Unlock(int argc, VALUE* argv, VALUE self) {
    if (!rb_block_given_p()) {
      return Qnil;
    }

    int state = 0;

    VALUE isolate, block;
    rb_scan_args(argc, argv, "10&", &isolate, &block);

    VALUE result = setupUnlockAndCall(Isolate(isolate), &state, block);

    if (state != 0) {
      rb_jump_tag(state);
    }

    return result;
  }

  VALUE Locker::setupUnlockAndCall(Isolate isolate, int* state, VALUE block) {
    v8::Unlocker unlocker(isolate);

    return rb_protect(&doUnlockCall, block, state);
  }

  VALUE Locker::doUnlockCall(VALUE block) {
    return rb_funcall(block, rb_intern("call"), 0);
  }

}
