#include "rr.h"

namespace rr {

Convert::Convert(v8::Handle<v8::Value> value) {
  this->value = value;
}
Convert::operator VALUE() {
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsExternal()) {
    return Qnil;
  }
  if (value->IsUint32()) {
    return UINT2NUM(value->Uint32Value());
  }
  if (value->IsInt32()) {
    return INT2FIX(value->Int32Value());
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  }
  if (value->IsNumber()) {
    return rb_float_new(value->NumberValue());
  }
  if (value->IsString()) {
    return String::Convert(value->ToString());
  }
  if (value->IsFunction()) {
    // return Function(value);
  }
  if (value->IsArray()) {
    // return Array(value);
  }
  if (value->IsDate()) {
    // return rr_reflect_v8_date(value);
  }
  if (value->IsObject()) {
    // return Object(value);
  }
  return Qnil;
}
}