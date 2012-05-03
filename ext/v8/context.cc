#include "rr.h"

namespace rr {

  VALUE New(VALUE ContextClass) {
    v8::Persistent<v8::Context> context = v8::Context::New();
    Ref<v8::Context> ref = Context::create(context, ContextClass);
    context.Dispose();
    return ref;
  }

  VALUE Enter(VALUE self) {
    Context(self)->Enter();
    return Qnil;
  }

  VALUE Exit(VALUE self) {
    Context(self)->Exit();
    return Qnil;
  }

  void Context::Init() {
    ClassBuilder("Context").
      defineSingletonMethod("New", &New).
      defineMethod("Enter", &Enter).
      defineMethod("Exit", &Exit);
  }
}