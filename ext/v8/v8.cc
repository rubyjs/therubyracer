#include "rr.h"

namespace rr {
namespace {
  VALUE IdleNotification(VALUE self) {
    return v8::V8::IdleNotification() ? Qtrue : Qfalse;
  }
}
void V8::Init() {
  VALUE V8Class = defineClass("V8");
  RR_DEFINE_SINGLETON_METHOD(V8Class, "IdleNotification", &IdleNotification, 0);
}
}