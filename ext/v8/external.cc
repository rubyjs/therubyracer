#include "rr.h"
#include "external.h"

namespace rr {
  void External::Init() {
    ClassBuilder("External").
      defineSingletonMethod("New", &New).

      defineMethod("Value", &Value).

      store(&Class);
  }

  VALUE External::New(VALUE self, VALUE r_isolate, VALUE object) {
    Isolate isolate(r_isolate);
    Locker lock(isolate);

    return External(isolate, wrap(isolate, object));
  }

  VALUE External::Value(VALUE self) {
    External external(self);
    Locker lock(external);
    return unwrap(external);
  }
}
