#include "rr.h"

namespace rr {
namespace {

VALUE New(VALUE klass, VALUE source, VALUE filename) {
  v8::HandleScope scope;
  return Script::create(v8::Script::New(String(source), Value(filename)), klass);
}

VALUE Run(VALUE self) {
  v8::HandleScope scope;
  return Value(Script(self)->Run());
}

}

void Script::Init() {
  ClassBuilder("Script").
    defineSingletonMethod("New", &New).
    defineMethod("Run", &Run);
}

}