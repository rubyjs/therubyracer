#include "rr.h"

namespace rr {

void Value::Init() {
  ClassBuilder("Value").
    defineMethod("Equals", &Equals).
    defineMethod("StrictEquals", &StrictEquals);
}

VALUE Value::Equals(VALUE self, VALUE other) {
  return Convert(Value(self)->Equals(Value(other)));
}

VALUE Value::StrictEquals(VALUE self, VALUE other) {
  return Convert(Value(self)->StrictEquals(Value(other)));
}
}