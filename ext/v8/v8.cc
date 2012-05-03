#include "rr.h"

namespace rr {
namespace {
  VALUE IdleNotification(VALUE self) {
    return v8::V8::IdleNotification() ? Qtrue : Qfalse;
  }
}
void V8::Init() {
  ClassBuilder("V8").
    defineSingletonMethod("IdleNotification", &IdleNotification);
}
}