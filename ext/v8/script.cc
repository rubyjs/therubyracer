#include "rr.h"

namespace rr {

void Script::Init() {
  ClassBuilder("Script").
    defineSingletonMethod("New", &New).
    defineMethod("Run", &Run);
}

VALUE Script::New(VALUE klass, VALUE source, VALUE filename) {
  v8::HandleScope scope;
  return Script::create(v8::Script::New(String(source), Value(filename)), klass);
}

VALUE Script::Run(VALUE self) {
  v8::HandleScope scope;
  return Convert(Script(self)->Run());
}

} //namespace rr