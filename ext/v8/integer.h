// -*- mode: c++ -*-
#ifndef RR_INTEGER_H
#define RR_INTEGER_H

namespace rr {
  class Uint32 : public Ref<v8::Uint32> {
  public:
    Uint32(VALUE self) :
      Ref<v8::Uint32>(self) {}
    Uint32(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Uint32>(isolate, value.As<v8::Uint32>()) {}

    static VALUE Value(VALUE self) {
      Uint32 uint32(self);
      Locker lock(uint32);

      return UINT2NUM(uint32->Value());
    }
  };

  class Int32 : public Ref<v8::Int32> {
  public:
    Int32(VALUE self) :
      Ref<v8::Int32>(self) {}
    Int32(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Int32>(isolate, value.As<v8::Int32>()) {}

    static VALUE Value(VALUE self) {
      Int32 int32(self);
      Locker lock(int32);

      return INT2NUM(int32->Value());
    }
  };

  class Integer : public Ref<v8::Integer> {
  public:
    Integer(v8::Isolate* isolate, v8::Handle<v8::Integer> integer) :
      Ref<v8::Integer>(isolate, integer) {}
    Integer(VALUE self) :
      Ref<v8::Integer>(self) {}

    static VALUE New(VALUE self, VALUE r_isolate, VALUE value) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      v8::Local<v8::Integer> integer(v8::Integer::New(isolate, NUM2INT(value)));
      if (integer->IsUint32()) {
        return Uint32(isolate, integer);
      } else if (integer->IsInt32()) {
        return Int32(isolate, integer);
      } else {
        return Integer(isolate, integer);
      }
    }

    static VALUE NewFromUnsigned(VALUE self, VALUE r_isolate, VALUE value) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      return Uint32(isolate, v8::Integer::NewFromUnsigned(isolate, NUM2UINT(value)));
    }

    static VALUE Value(VALUE self) {
      Integer integer(self);
      Locker lock(integer);

      return INT2NUM(integer->Value());
    }

    static void Init() {
      ClassBuilder("Integer", Number::Class).
        defineSingletonMethod("New", &New).
        defineSingletonMethod("NewFromUnsigned", &NewFromUnsigned).
        defineMethod("Value", &Value).
        store(&Class);

      ClassBuilder("Int32", Integer::Class).
        defineMethod("Value", &Value).
        store(&Int32::Class);

      ClassBuilder("Uint32", Integer::Class).
        defineMethod("Value", &Value).
        store(&Uint32::Class);
    }
  };
}

#endif /* RR_INTEGER_H */
