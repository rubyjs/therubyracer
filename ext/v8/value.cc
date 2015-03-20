#include "rr.h"

namespace rr {

  VALUE Value::Empty;

  void Value::Init() {
    Empty = rb_eval_string("Object.new");

    ClassBuilder("Value").
      defineConst("Empty", Empty).

      defineMethod("IsUndefined", &IsUndefined).
      defineMethod("IsNull", &IsNull).
      defineMethod("IsTrue", &IsTrue).
      defineMethod("IsFalse", &IsFalse).
      // defineMethod("IsString", &IsString).
      // defineMethod("IsFunction", &IsFunction).
      // defineMethod("IsArray", &IsArray).
      defineMethod("IsObject", &IsObject).
      // defineMethod("IsBoolean", &IsBoolean).
      // defineMethod("IsNumber", &IsNumber).
      // defineMethod("IsExternal", &IsExternal).
      // defineMethod("IsInt32", &IsInt32).
      // defineMethod("IsUint32", &IsUint32).
      // defineMethod("IsDate", &IsDate).
      // defineMethod("IsBooleanObject", &IsBooleanObject).
      // defineMethod("IsNumberObject", &IsNumberObject).
      // defineMethod("IsStringObject", &IsStringObject).
      // defineMethod("IsNativeError", &IsNativeError).
      // defineMethod("IsRegExp", &IsRegExp).
      // defineMethod("ToString", &ToString).
      // defineMethod("ToDetailString", &ToDetailString).
      // defineMethod("ToObject", &ToObject).
      // defineMethod("BooleanValue", &BooleanValue).
      // defineMethod("NumberValue", &NumberValue).
      // defineMethod("IntegerValue", &IntegerValue).
      // defineMethod("Uint32Value", &Uint32Value).
      // defineMethod("IntegerValue", &IntegerValue).
      // defineMethod("Equals", &Equals).
      // defineMethod("StrictEquals", &StrictEquals).

      store(&Class);

      rb_gc_register_address(&Empty);
  }

  VALUE Value::IsUndefined(VALUE self) {
    return Bool(Value(self)->IsUndefined());
  }

  VALUE Value::IsNull(VALUE self) {
    return Bool(Value(self)->IsNull());
  }

  VALUE Value::IsTrue(VALUE self) {
    return Bool(Value(self)->IsTrue());
  }

  VALUE Value::IsFalse(VALUE self) {
    return Bool(Value(self)->IsFalse());
  }

  VALUE Value::IsObject(VALUE self) {
    return Bool(Value(self)->IsObject());
  }

  VALUE Value::Equals(VALUE self, VALUE other) {
    return Bool(Value(self)->Equals(Value(other)));
  }

  VALUE Value::StrictEquals(VALUE self, VALUE other) {
    return Bool(Value(self)->StrictEquals(Value(other)));
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

    // TODO
    // if (handle->IsExternal()) {
    //   return External((v8::Handle<v8::External>)v8::External::Cast(*handle));
    // }

    // TODO
    // if (handle->IsUint32()) {
    //   return UInt32(handle->Uint32Value());
    // }
    //
    // if (handle->IsInt32()) {
    //   return INT2FIX(handle->Int32Value());
    // }

    if (handle->IsBoolean()) {
      return handle->BooleanValue() ? Qtrue : Qfalse;
    }

    // TODO
    // if (handle->IsNumber()) {
    //   return rb_float_new(handle->NumberValue());
    // }

    // TODO
    // if (handle->IsString()) {
    //   return String(handle->ToString());
    // }
    //
    // if (handle->IsDate()) {
    //   return Date((v8::Handle<v8::Date>)v8::Date::Cast(*handle));
    // }

    if (handle->IsObject()) {
      return Object(handle->ToObject());
    }

    return Ref<v8::Value>::operator VALUE();
  }

  Value::operator v8::Handle<v8::Value>() const {
    if (rb_equal(value, Empty)) {
      return v8::Handle<v8::Value>();
    }

    v8::Isolate* isolate = v8::Isolate::GetCurrent();

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
      return Ref<v8::Value>::operator v8::Handle<v8::Value>();
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

}
