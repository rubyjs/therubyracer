#include "rr.h"

namespace rr {

  void Isolate::Init() {
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).

      defineMethod("Equals", &rr::Isolate::PointerEquals).

      store(&Class);
  }

  VALUE Isolate::New(VALUE self) {
    return Isolate(v8::Isolate::New());
  }

  template <>
  void Pointer<v8::Isolate>::unwrap(VALUE value) {
    Data_Get_Struct(value, class v8::Isolate, pointer);
  }

}
