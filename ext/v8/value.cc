#include "rr.h"

namespace rr {
  Value::Value(v8::Handle<v8::Value> value) {
    this->value = value;
  }
  Value::Value(VALUE object) {
    this->object = object;
  }
  Value::operator VALUE() {
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
      return String::Value(value);
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
  Value::operator v8::Handle<v8::Value>() {
      switch (TYPE(object)) {
      case T_FIXNUM:
        // TODO: use this conversion if value will fit in 32 bits.
        // return Integer::New(FIX2LONG(value));
      case T_FLOAT:
        // return Number::New(NUM2DBL(value));
      case T_STRING:
      return (v8::Handle<v8::String>)String(object);
      case T_NIL:
        return v8::Null();
      case T_TRUE:
        return v8::True();
      case T_FALSE:
        return v8::False();
      case T_DATA:
      return Ref<v8::Value>(object);
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
    //  case T_BLKTAG: (not in 1.9)
      case T_UNDEF:
    //  case T_VARMAP: (not in 1.9)
    //  case T_SCOPE: (not in 1.9)
      case T_NODE:
      default:
        rb_warn("unknown conversion to V8 for: %s", RSTRING_PTR(rb_inspect(object)));
        return v8::String::New("Undefined Conversion");
      }
      return v8::Undefined();
  }
}