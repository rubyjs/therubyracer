#include "v8_value.h"
#include "v8_ref.h"

using namespace v8;
namespace {
  Local<Value> unwrap(VALUE value) {
    return V8_Ref_Get<Value>(value);
  }
  VALUE IsEmpty(VALUE value) {
    return value == rr_cV8_C_Empty ? Qtrue : Qfalse;
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

VALUE rr_cV8_C_Value;
VALUE rr_cV8_C_Empty;

void rr_init_value() {
  rr_cV8_C_Value = rr_define_class("Value");
  rr_cV8_C_Empty = rr_define_const("Empty", rr_v8_ref_create(rr_cV8_C_Value, Handle<Value>()));

  rr_define_method(rr_cV8_C_Value, "IsEmpty", IsEmpty, 0);
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

  rr_define_method(rr_cV8_C_Value, "BooleanValue", BooleanValue, 0);
  rr_define_method(rr_cV8_C_Value, "NumberValue", NumberValue, 0);
}

VALUE rr_wrap_v8_value(Handle<Value>& value) {
  return rr_v8_ref_create(rr_cV8_C_Value, value);
}