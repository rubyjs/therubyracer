#include "rr.h"

namespace rr {

  void Array::Init() {
    ClassBuilder("Array", Object::Class).

      defineSingletonMethod("New", &New).

      defineMethod("Length", &Length).
      defineMethod("CloneElementAt", &CloneElementAt).

      store(&Class);
  }

  VALUE Array::New(int argc, VALUE argv[], VALUE self) {
    VALUE rb_isolate, length;
    rb_scan_args(argc, argv, "11", &rb_isolate, &length);

    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Array(isolate, v8::Array::New(isolate, RTEST(length) ? NUM2INT(length) : 0));
  }

  VALUE Array::Length(VALUE self) {
    Array array(self);
    Locker lock(array.getIsolate());

    return Uint32_t(array->Length());
  }

  VALUE Array::CloneElementAt(VALUE self, VALUE index) {
    Array array(self);
    Locker lock(array.getIsolate());

    return Object(array.getIsolate(), array->CloneElementAt(Uint32_t(index)));
  }

}
