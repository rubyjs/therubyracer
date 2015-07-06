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
    isolate.retainObject(object);

    Locker lock(isolate);

    Container* container = new Container(object);
    v8::Local<v8::External> external(v8::External::New(isolate, (void*)container));

    v8::Global<v8::External>* global(new v8::Global<v8::External>(isolate, external));
    container->global = global;

    global->SetWeak<Container>(container, &release, v8::WeakCallbackType::kParameter);

    return External(isolate, external);
  }

  VALUE External::Value(VALUE self) {
    External external(self);
    Locker lock(external);
    Container* container((Container*)external->Value());
    return container->object;
  }
}
