#ifndef RR_LOCKER
#define RR_LOCKER

namespace rr {

  class Locker {
  public:
    static void Init();

    static VALUE IsLocked(VALUE self, VALUE isolate);
    static VALUE IsActive(VALUE self);

    static VALUE Lock(int argc, VALUE* argv, VALUE self);
    static VALUE Unlock(int argc, VALUE* argv, VALUE self);

  protected:
    static VALUE setupLockAndCall(Isolate isolate, int* state, VALUE code);
    static VALUE doLockCall(VALUE code);

    static VALUE setupUnlockAndCall(Isolate isolate, int* state, VALUE code);
    static VALUE doUnlockCall(VALUE code);
  };

}

#endif
