#include "rr.h"

namespace rr {

void V8::Init() {
  ClassBuilder("V8").
    defineSingletonMethod("IdleNotification", &V8::IdleNotification);
}

VALUE V8::IdleNotification(VALUE self) {
  return v8::V8::IdleNotification() ? Qtrue : Qfalse;
}

}