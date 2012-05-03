#include "rr.h"

namespace rr {

void Context::Init() {
  ClassBuilder("Context").
    defineSingletonMethod("New", &New).
    defineMethod("Enter", &Enter).
    defineMethod("Exit", &Exit);
}

VALUE Context::New(VALUE ContextClass) {
  v8::Persistent<v8::Context> context = v8::Context::New();
  Ref<v8::Context> ref = Context::create(context, ContextClass);
  context.Dispose();
  return ref;
}

VALUE Context::Enter(VALUE self) {
  Context(self)->Enter();
  return Qnil;
}

VALUE Context::Exit(VALUE self) {
  Context(self)->Exit();
  return Qnil;
}

}
