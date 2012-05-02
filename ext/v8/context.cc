#include "rr.h"

namespace rr {

  VALUE New(VALUE ContextClass) {
    v8::Persistent<v8::Context> context = v8::Context::New();
    Ref<v8::Context> ref = Ref<v8::Context>::create(context, ContextClass);
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
    VALUE ContextClass = defineClass("Context");
    RR_DEFINE_SINGLETON_METHOD(ContextClass, "New", &New, 0);
    RR_DEFINE_METHOD(ContextClass, "Enter", &Enter, 0);
    RR_DEFINE_METHOD(ContextClass, "Exit", &Exit, 0);
  }
}