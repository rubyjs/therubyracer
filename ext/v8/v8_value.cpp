#include "v8_value.h"
#include "v8_ref.h"

using namespace v8;

Local<Value> unwrap(VALUE value) {
  return V8_Ref_Get<Value>(value);
}

VALUE IsUndefined(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsUndefined());
}
VALUE IsNull(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsNull());
}
VALUE IsTrue(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsTrue());
}
VALUE IsFalse(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsFalse());
}
VALUE IsString(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsString());
}
VALUE IsFunction(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsFunction());
}
VALUE IsArray(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsArray());
}
VALUE IsObject(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsObject());
}
VALUE IsBoolean(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsBoolean());
}
VALUE IsNumber(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsNumber());
}
VALUE IsExternal(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsExternal());
}
VALUE IsInt32(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsInt32());
}
VALUE IsUint32(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsUint32());
}
VALUE IsDate(VALUE self) {
  return rr_to_ruby(unwrap(self)->IsDate());
}

VALUE ToBoolean(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToBoolean());
}
VALUE ToNumber(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToNumber());
}
VALUE ToString(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToString());
}
VALUE ToDetailString(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToDetailString());
}
VALUE ToObject(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToObject());
}
VALUE ToInteger(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToInteger());
}
VALUE ToUint32(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToUint32());
}
VALUE ToInt32(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToInt32());
}
VALUE ToArrayIndex(VALUE self) {
  return rr_to_ruby(unwrap(self)->ToArrayIndex());
}

VALUE Equals(VALUE self, VALUE that) {
  return rr_to_ruby(unwrap(self)->Equals(unwrap(that)));
}
VALUE StrictEquals(VALUE self, VALUE that) {
  return rr_to_ruby(unwrap(self)->StrictEquals(unwrap(that)));
}

VALUE rr_cV8_C_Value;

void rr_init_v8_value() {
  rr_cV8_C_Value = rr_define_class("Value");
  rr_define_method(rr_cV8_C_Value, "IsUndefined", IsUndefined, 0);
  rr_define_method(rr_cV8_C_Value, "IsNull", IsNull, 0);
  rr_define_method(rr_cV8_C_Value, "IsTrue", IsTrue, 0);
  rr_define_method(rr_cV8_C_Value, "IsFalse", IsFalse, 0);
  rr_define_method(rr_cV8_C_Value, "IsString", IsString, 0);
  rr_define_method(rr_cV8_C_Value, "IsFunction", IsFunction, 0);
  rr_define_method(rr_cV8_C_Value, "IsArray", IsArray, 0);
  rr_define_method(rr_cV8_C_Value, "IsObject", IsObject, 0);
  rr_define_method(rr_cV8_C_Value, "IsBoolean", IsBoolean, 0);
  rr_define_method(rr_cV8_C_Value, "IsNumber", IsNumber, 0);
  rr_define_method(rr_cV8_C_Value, "IsExternal", IsExternal, 0);
  rr_define_method(rr_cV8_C_Value, "IsInt32", IsInt32, 0);
  rr_define_method(rr_cV8_C_Value, "IsUint32", IsUint32, 0);
  rr_define_method(rr_cV8_C_Value, "IsDate", IsDate, 0);

  rr_define_method(rr_cV8_C_Value, "ToBoolean", ToBoolean, 0);
  rr_define_method(rr_cV8_C_Value, "ToNumber", ToNumber, 0);
  rr_define_method(rr_cV8_C_Value, "ToString", ToString, 0);
  rr_define_method(rr_cV8_C_Value, "ToDetailString", ToDetailString, 0);
  rr_define_method(rr_cV8_C_Value, "ToObject", ToObject, 0);
  rr_define_method(rr_cV8_C_Value, "ToInteger", ToInteger, 0);
  rr_define_method(rr_cV8_C_Value, "ToUint32", ToUint32, 0);
  rr_define_method(rr_cV8_C_Value, "ToArrayIndex", ToArrayIndex, 0);
  
  rr_define_method(rr_cV8_C_Value, "Equals", Equals, 1);
  rr_define_method(rr_cV8_C_Value, "StrictEquals", StrictEquals, 1);
}

VALUE rr_wrap_v8_value(Handle<Value>& value) {
  return V8_Ref_Create(rr_cV8_C_Value, value);
}