// -*- mode: c++ -*-
#ifndef RR_LOCKER
#define RR_LOCKER

namespace rr {

  class Locker {
  public:
    Locker(v8::Isolate* _isolate): isolate(_isolate), locker(_isolate) {
      isolate->Enter();
    }

    ~Locker() {
      isolate->Exit();
    }

  protected:
    v8::Isolate* isolate;
    v8::Locker locker;
  };

  class Unlocker {
  public:
    Unlocker(v8::Isolate* isolate): unlocker(isolate) {}

  protected:
    v8::Unlocker unlocker;
  };

}

#endif
