#include "rr.h"
#include "v8_locker.h"

using namespace v8;

namespace {
  namespace Lock {

    /**
    * Document-method: V8::C::Locker#new
    *
    * Allocates and returns a new `v8::Locker` object. The thread that instantiated
    * this object will hold the V8 interpreter lock until it is released with a
    * corresponding call to {#delete}.
    *
    * It critical that you call {#delete} to deallocate it, preferably within the same method.
    * If you don't, two bad things will happen:
    *
    * 1. You'll leak the underlying C++ object
    * 1. Worse, you'll leave the V8 vm locked to this thread forever
    *
    * It's dangerous! Be sure to `ensure`.
    *
    * for detailed semantics see the locking {API http://izs.me/v8-docs/classv8_1_1Unlocker.html}
    *
    * @return [V8::C::Locker] the new locker
    */

    VALUE New(VALUE LockerClass) {
      Locker* locker = new Locker();
      return Data_Wrap_Struct(LockerClass, 0, 0, (void*)locker);
    }

    /**
    * Document-method: V8::C::Locker#delete
    *
    * Pop this lock off the stack for this thread. For a full run down of V8 locking
    * semantics see the locking {API http://izs.me/v8-docs/classv8_1_1Unlocker.html}
    * @return nil
    */
    VALUE Delete(VALUE self) {
      Locker* locker = 0;
      Data_Get_Struct(self, class Locker, locker);
      delete locker;
    }
  }

  namespace Unlock {
    /**
    * Document-method: V8::C::Unlocker#new
    *
    * Allocates and returns a new `v8::UnLocker` object, temporarily releasing any locks that
    * this thread is holding. It will reaquire all of the locksto {#delete}.
    *
    * This is a great thing to do when you want to call out to some code that might do some
    * waiting, sleeping, and you want to politely let other threads use this VM.
    *
    * It critical that you call {#delete} to deallocate it, preferably within the same method.
    * If you don't, two bad things will happen:
    *
    * 1. You'll leak the underlying C++ object
    * 1. You won't restore the locks to your current thread, and will mess things up horribly
    *
    * It's dangerous! Be sure to `ensure`.
    *
    * For details on V8 locking semantics, see the locking {API http://izs.me/v8-docs/classv8_1_1Unlocker.html}
    * @return [V8::C::Unocker] the new locker
    */
    VALUE New(VALUE UnlockerClass) {
      Unlocker* unlocker = new Unlocker();
      return Data_Wrap_Struct(UnlockerClass, 0, 0, (void*)unlocker);
    }

    /**
    * Document-method: V8::C::Unlocker#delete
    *
    * Restore any locks to the stack that were temporarily removed by this `Unlocker`.
    * For a full run down, see semantics see the locking {API http://izs.me/v8-docs/classv8_1_1Unlocker.html}
    * @return nil
    */
    VALUE Delete(VALUE self) {
      Unlocker* unlocker;
      Data_Get_Struct(self, class Unlocker, unlocker);
      delete unlocker;
    }
  }

  /**
  * Document-method: V8::C::Locker#StartPreemption
  * Start preemption.
  * When preemption is started, a timer is fired every n milli seconds that will switch between
  * multiple threads that are in contention for the V8 lock.
  *
  * @param [Integer] every_n_ms
  * @return nil
  */
  VALUE StartPreemption(VALUE self, VALUE every_n_ms) {
    Locker::StartPreemption(NUM2INT(rb_to_int(every_n_ms)));
    return Qnil;
  }

  /**
  * Document-method: V8::C::Locker#StartPreemption
  * Stop preemption
  */
  VALUE StopPreemption(VALUE self) {
    Locker::StopPreemption();
    return Qnil;
  }

  /**
  * Document-method: V8::C::Locker#IsLocked
  * Returns whether or not the locker is locked by the current thread.
  */
  VALUE IsLocked(VALUE self) {
    return rr_v82rb(Locker::IsLocked());
  }

  /**
  * Document-method: V8::C::Locker#IsActive
  * Returns whether v8::Locker is being used by this V8 instance.
  */
  VALUE IsActive(VALUE self) {
    return rr_v82rb(Locker::IsActive());
  }
}

void rr_init_v8_locker() {
  VALUE LockerClass = rr_define_class("Locker");
  VALUE UnlockerClass = rr_define_class("Unlocker");
  rr_define_singleton_method(LockerClass, "new", Lock::New, 0);
  rr_define_method(LockerClass, "delete", Lock::Delete, 0);
  rr_define_singleton_method(UnlockerClass, "new", Unlock::New, 0);
  rr_define_method(UnlockerClass, "delete", Unlock::Delete, 0);
  rr_define_singleton_method(LockerClass, "StartPreemption", StartPreemption, 1);
  rr_define_singleton_method(LockerClass, "StopPreemption", StopPreemption, 0);
  rr_define_singleton_method(LockerClass, "IsLocked", IsLocked, 0);
  rr_define_singleton_method(LockerClass, "IsActive", IsActive, 0);
}
