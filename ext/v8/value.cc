#include "rr.h"

namespace rr {

  void Value::Init() {
    ClassBuilder("Value").
      defineMethod("IsUndefined", &IsUndefined).
      defineMethod("IsNull", &IsNull).
      defineMethod("IsTrue", &IsTrue).
      defineMethod("IsFalse", &IsFalse).
      defineMethod("IsBoolean", &IsBoolean).
      defineMethod("IsString", &IsString).
      defineMethod("IsFunction", &IsFunction).
      // defineMethod("IsArray", &IsArray).
      defineMethod("IsObject", &IsObject).
      defineMethod("IsNumber", &IsNumber).
      defineMethod("IsExternal", &IsExternal).
      defineMethod("IsInt32", &IsInt32).
      defineMethod("IsUint32", &IsUint32).
      // defineMethod("IsDate", &IsDate).
      // defineMethod("IsBooleanObject", &IsBooleanObject).
      // defineMethod("IsNumberObject", &IsNumberObject).
      // defineMethod("IsStringObject", &IsStringObject).
      defineMethod("IsNativeError", &IsNativeError).
      // defineMethod("IsRegExp", &IsRegExp).
      defineMethod("ToString", &ToString).
      // defineMethod("ToDetailString", &ToDetailString).
      // defineMethod("ToObject", &ToObject).
      defineMethod("BooleanValue", &BooleanValue).
      // defineMethod("NumberValue", &NumberValue).
      // defineMethod("IntegerValue", &IntegerValue).
      // defineMethod("Uint32Value", &Uint32Value).
      // defineMethod("IntegerValue", &IntegerValue).
      defineMethod("Equals", &Equals).
      defineMethod("StrictEquals", &StrictEquals).

      store(&Class);
  }

  VALUE Value::IsUndefined(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsUndefined());
  }

  VALUE Value::IsNull(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsNull());
  }

  VALUE Value::IsTrue(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsTrue());
  }

  VALUE Value::IsFalse(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsFalse());
  }

  VALUE Value::IsBoolean(VALUE self) {
    Value value(self);
    Locker lock(value);

    return Bool(value->IsBoolean());
  }

  VALUE Value::IsString(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsString());
  }

  VALUE Value::IsFunction(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsFunction());
  }

  VALUE Value::IsObject(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsObject());
  }

  VALUE Value::IsExternal(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsExternal());
  }

  VALUE Value::IsNumber(VALUE self) {
    Value value(self);
    Locker lock(value);
    return Bool(value->IsNumber());
  }

  VALUE Value::IsInt32(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsInt32());
  }

  VALUE Value::IsUint32(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->IsUint32());
  }

  VALUE Value::IsNativeError(VALUE self) {
    Value value(self);
    Locker lock(value);

    return Bool(value->IsNativeError());
  }

  VALUE Value::ToString(VALUE self) {
    Value value(self);
    Locker lock(value.getIsolate());

    return String(value.getIsolate(), value->ToString());
  }

  VALUE Value::BooleanValue(VALUE self, VALUE r_context) {
    Value value(self);
    Locker lock(value);

    return Bool::Maybe(value->BooleanValue(Context(r_context)));
  }

  VALUE Value::Equals(VALUE self, VALUE other) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->Equals(Value(other)));
  }

  VALUE Value::StrictEquals(VALUE self, VALUE other) {
    Value value(self);
    Locker lock(value.getIsolate());

    return Bool(value->StrictEquals(Value(other)));
  }

  Value::operator VALUE() {
    if (handle.IsEmpty()) {
      return Qnil;
    }

    if (handle->IsUndefined()) {
      return Undefined(isolate, handle);
    }

    if (handle->IsNull()) {
      return Null(isolate, handle);
    }

    if (handle->IsBoolean()) {
      return Boolean(isolate, handle.As<v8::Boolean>());
    }

    if (handle->IsExternal()) {
      return External(isolate, handle);
    }

    if (handle->IsUint32()) {
      return Uint32(isolate, handle);
    }

    if (handle->IsInt32()) {
      return Int32(isolate, handle);
    }

    if (handle->IsNumber()) {
      return Number(isolate, handle);
    }

    if (handle->IsString()) {
      return String(isolate, handle.As<v8::String>());
    }

    if (handle->IsSymbol()) {
      return Symbol(isolate, handle.As<v8::Symbol>());
    }

    if (handle->IsName()) {
      return Name(isolate, handle.As<v8::Name>());
    }

    if (handle->IsFunction()) {
      return Function(isolate, v8::Handle<v8::Function>::Cast(handle));
    }

    if (handle->IsObject()) {
      return Object(isolate, handle->ToObject());
    }
    return Ref<v8::Value>::operator VALUE();
  }

  v8::Handle<v8::Value> Value::rubyObjectToHandle(v8::Isolate* isolate, VALUE value) {
    switch (TYPE(value)) {
    case T_FIXNUM:
      return v8::Integer::New(isolate, NUM2INT(value));
    case T_FLOAT:
      return v8::Number::New(isolate, NUM2DBL(value));
    case T_STRING:
      return v8::String::NewFromUtf8(isolate, RSTRING_PTR(value), v8::String::kNormalString, (int)RSTRING_LEN(value));
    case T_NIL:
      return v8::Null(isolate);
    case T_TRUE:
      return v8::True(isolate);
    case T_FALSE:
      return v8::False(isolate);
    case T_DATA:
      return Value(value);
    case T_OBJECT:
    case T_CLASS:
    case T_ICLASS:
    case T_MODULE:
    case T_REGEXP:
    case T_MATCH:
    case T_ARRAY:
    case T_HASH:
    case T_STRUCT:
    case T_BIGNUM:
    case T_FILE:
    case T_SYMBOL:
    case T_UNDEF:
    case T_NODE:
    default:
      rb_warn("unknown conversion to V8 for: %s", RSTRING_PTR(rb_inspect(value)));
      return v8::String::NewFromUtf8(isolate, "Undefined Conversion");
    }

    return v8::Undefined(isolate);
  }

  std::vector< v8::Handle<v8::Value> > Value::convertRubyArray(v8::Isolate* isolate, VALUE value) {
    std::vector< v8::Handle<v8::Value> > vector(RARRAY_LENINT(value));

    for (uint32_t i = 0; i < vector.size(); i++) {
      vector[i] = Value::rubyObjectToHandle(isolate, rb_ary_entry(value, i));
    }

    return vector;
  }

}
