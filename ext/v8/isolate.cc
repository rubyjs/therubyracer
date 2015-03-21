#include "rr.h"

namespace rr {

  void Isolate::Init() {
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).
      defineSingletonMethod("GetCurrent", &GetCurrent).

      defineMethod("Enter", &Enter).
      defineMethod("Exit", &Exit).
      defineMethod("Equals", &rr::Isolate::PointerEquals).

      store(&Class);
  }

  VALUE Isolate::New(VALUE self) {
    return Isolate(v8::Isolate::New());
  }

  VALUE Isolate::Enter(VALUE self) {
    Isolate(self)->Enter();
    return Qtrue;
  }

  VALUE Isolate::Exit(VALUE self) {
    Isolate(self)->Exit();
    return Qtrue;
  }

  VALUE Isolate::GetCurrent(VALUE self) {
    v8::Isolate* currentIsolate = v8::Isolate::GetCurrent();

    if (!currentIsolate) {
      return Qnil;
    }

    return Isolate(currentIsolate);
  }

  template <>
  void Pointer<v8::Isolate>::unwrap(VALUE value) {
    Data_Get_Struct(value, class v8::Isolate, pointer);
  }

}
