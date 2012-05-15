#include "rr.h"

namespace rr {

void V8::Init() {
  ClassBuilder("V8").
    defineSingletonMethod("IdleNotification", &IdleNotification);
}

VALUE V8::IdleNotification(VALUE self) {
  return Bool(v8::V8::IdleNotification());
}

}