// -*- mode: c++ -*-
#include "rr.h"
#include "isolate.h"

namespace rr {

  void Isolate::Init() {
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).

      defineMethod("Dispose", &Isolate::Dispose).
      defineMethod("Equals", &rr::Isolate::PointerEquals).

      store(&Class);
  }

  VALUE Isolate::New(VALUE self) {
    return Isolate(v8::Isolate::New());
  }

  VALUE Isolate::Dispose(VALUE self) {
    Isolate(self)->Dispose();
    return Qnil;
  }

  template <>
  void Pointer<v8::Isolate>::unwrap(VALUE value) {
    Data_Get_Struct(value, class v8::Isolate, pointer);
  }
}
