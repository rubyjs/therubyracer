#include "rr.h"

namespace rr {

void Value::Init() {
  ClassBuilder("Value").
    defineMethod("Equals", &Equals).
    defineMethod("StrictEquals", &StrictEquals)
    .store(&Class);
}

VALUE Value::Equals(VALUE self, VALUE other) {
  return Convert(Value(self)->Equals(Value(other)));
}

VALUE Value::StrictEquals(VALUE self, VALUE other) {
  return Convert(Value(self)->StrictEquals(Value(other)));
}

Value::operator VALUE() {
  if (handle.IsEmpty() || handle->IsUndefined() || handle->IsNull()) {
    return Qnil;
  }
  if (handle->IsTrue()) {
    return Qtrue;
  }
  if (handle->IsFalse()) {
    return Qfalse;
  }
  if (handle->IsExternal()) {
    return External((v8::Handle<v8::External>)v8::External::Cast(*handle));
  }
  if (handle->IsUint32()) {
    return UINT2NUM(handle->Uint32Value());
  }
  if (handle->IsInt32()) {
    return INT2FIX(handle->Int32Value());
  }
  if (handle->IsBoolean()) {
    return handle->BooleanValue() ? Qtrue : Qfalse;
  }
  if (handle->IsNumber()) {
    return rb_float_new(handle->NumberValue());
  }
  if (handle->IsString()) {
    return String(handle->ToString());
  }
  // if (handle->IsFunction()) {
  //   // return Function(handle);
  // }
  // if (handle->IsArray()) {
  //   // return Array(handle);
  // }
  // if (handle->IsDate()) {
  //   // return rr_reflect_v8_date(handle);
  // }
  if (handle->IsObject()) {
    return Object(handle->ToObject());
  }
  return Ref<v8::Value>::operator VALUE();
}
}