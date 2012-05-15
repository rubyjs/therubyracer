#include "rr.h"

namespace rr {

void Script::Init() {
  ClassBuilder("Script").
    defineSingletonMethod("New", &New).
    defineMethod("Run", &Run).
    store(&Class);
}

VALUE Script::New(VALUE klass, VALUE source, VALUE filename) {
  return Script(v8::Script::New(String(source), Value(filename)));
}

VALUE Script::Run(VALUE self) {
  return Value(Script(self)->Run());
}

} //namespace rr