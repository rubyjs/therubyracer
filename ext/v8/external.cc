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

    // as long as this external is alive within JavaScript, it should not be
    // garbage collected by Ruby.
    isolate.retainObject(object);

    Locker lock(isolate);

    // create the external.
    Container* container = new Container(object);
    v8::Local<v8::External> external(v8::External::New(isolate, (void*)container));

    // next, we create a weak reference to this external so that we can be
    // notified when V8 is done with it. At that point, we can let Ruby know
    // that this external is done with it.
    v8::Global<v8::External>* global(new v8::Global<v8::External>(isolate, external));
    global->SetWeak<Container>(container, &release, v8::WeakCallbackType::kParameter);
    container->global = global;

    return External(isolate, external);
  }

  VALUE External::Value(VALUE self) {
    External external(self);
    Locker lock(external);
    Container* container((Container*)external->Value());
    return container->object;
  }
}
