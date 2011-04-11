#include "v8_handle.h"
#include "v8_value.h"

using namespace v8;
namespace {

  VALUE ValueClass;

  Persistent<Value>& unwrap(VALUE value) {
    return rr_v8_handle<Value>(value);
  }
  VALUE IsEmpty(VALUE value) {
    return value == rr_v8_value_empty() ? Qtrue : Qfalse;
  }
  VALUE IsUndefined(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsUndefined());
  }
  VALUE IsNull(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsNull());
  }
  VALUE IsTrue(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsTrue());
  }
  VALUE IsFalse(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsFalse());
  }
  VALUE IsString(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsString());
  }
  VALUE IsFunction(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsFunction());
  }
  VALUE IsArray(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsArray());
  }
  VALUE IsObject(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsObject());
  }
  VALUE IsBoolean(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsBoolean());
  }
  VALUE IsNumber(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsNumber());
  }
  VALUE IsExternal(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsExternal());
  }
  VALUE IsInt32(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsInt32());
  }
  VALUE IsUint32(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsUint32());
  }
  VALUE IsDate(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->IsDate());
  }

  VALUE ToBoolean(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToBoolean());
  }
  VALUE ToNumber(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToNumber());
  }
  VALUE ToString(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToString());
  }
  VALUE ToDetailString(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToDetailString());
  }
  VALUE ToObject(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToObject());
  }
  VALUE ToInteger(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToInteger());
  }
  VALUE ToUint32(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToUint32());
  }
  VALUE ToInt32(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToInt32());
  }
  VALUE ToArrayIndex(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->ToArrayIndex());
  }

  VALUE Equals(VALUE self, VALUE that) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->Equals(unwrap(that)));
  }
  VALUE StrictEquals(VALUE self, VALUE that) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->StrictEquals(unwrap(that)));
  }

  VALUE BooleanValue(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->BooleanValue());
  }

  VALUE NumberValue(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->NumberValue());
  }  
}

VALUE rr_v8_value_class() {
  return ValueClass;
}

VALUE rr_v8_value_empty() {
  return rr_const_get("Empty");
}

void rr_init_value() {
  ValueClass = rr_define_class("Value", rr_v8_handle_class());
  rr_define_const("Empty", rr_v8_handle_new(ValueClass, Handle<Value>()));

  rr_define_method(ValueClass, "IsEmpty", IsEmpty, 0);
  rr_define_method(ValueClass, "IsUndefined", IsUndefined, 0);
  rr_define_method(ValueClass, "IsNull", IsNull, 0);
  rr_define_method(ValueClass, "IsTrue", IsTrue, 0);
  rr_define_method(ValueClass, "IsFalse", IsFalse, 0);
  rr_define_method(ValueClass, "IsString", IsString, 0);
  rr_define_method(ValueClass, "IsFunction", IsFunction, 0);
  rr_define_method(ValueClass, "IsArray", IsArray, 0);
  rr_define_method(ValueClass, "IsObject", IsObject, 0);
  rr_define_method(ValueClass, "IsBoolean", IsBoolean, 0);
  rr_define_method(ValueClass, "IsNumber", IsNumber, 0);
  rr_define_method(ValueClass, "IsExternal", IsExternal, 0);
  rr_define_method(ValueClass, "IsInt32", IsInt32, 0);
  rr_define_method(ValueClass, "IsUint32", IsUint32, 0);
  rr_define_method(ValueClass, "IsDate", IsDate, 0);

  rr_define_method(ValueClass, "ToBoolean", ToBoolean, 0);
  rr_define_method(ValueClass, "ToNumber", ToNumber, 0);
  rr_define_method(ValueClass, "ToString", ToString, 0);
  rr_define_method(ValueClass, "ToDetailString", ToDetailString, 0);
  rr_define_method(ValueClass, "ToObject", ToObject, 0);
  rr_define_method(ValueClass, "ToInteger", ToInteger, 0);
  rr_define_method(ValueClass, "ToUint32", ToUint32, 0);
  rr_define_method(ValueClass, "ToArrayIndex", ToArrayIndex, 0);

  rr_define_method(ValueClass, "Equals", Equals, 1);
  rr_define_method(ValueClass, "StrictEquals", StrictEquals, 1);

  rr_define_method(ValueClass, "BooleanValue", BooleanValue, 0);
  rr_define_method(ValueClass, "NumberValue", NumberValue, 0);
}

VALUE rr_wrap_v8_value(Handle<Value>& value) {
  return rr_v8_handle_new(ValueClass, value);
}