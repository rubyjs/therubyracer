#include "rr.h"

namespace rr {

void Object::Init() {
  ClassBuilder("Object", "Value").
    defineSingletonMethod("New", &New);
}

VALUE Object::New(VALUE self) {
  return Object::create(v8::Object::New(), self);
}

}