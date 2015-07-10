#include "rr.h"

namespace rr {
  void Number::Init() {
    ClassBuilder("Number", Primitive::Class).

      defineSingletonMethod("New", [] (VALUE self, VALUE r_isolate, VALUE value) -> VALUE {
          Isolate isolate(r_isolate);
          Locker lock(isolate);

          return Number(isolate, NUM2DBL(value));
        }).

      defineMethod("Value", [] (VALUE self) -> VALUE {
          Number number(self);
          Locker lock(number);

          return DBL2NUM(number->Value());
        }).

      store(&Class);

    ClassBuilder("Integer", Number::Class).

      defineSingletonMethod("New", [] (VALUE self, VALUE r_isolate, VALUE value) -> VALUE {
          Isolate isolate(r_isolate);
          Locker lock(isolate);

          auto integer = v8::Integer::New(isolate, NUM2INT(value));
          return Value::handleToRubyObject(isolate, integer);
        }).

      defineSingletonMethod("NewFromUnsigned", [] (VALUE self, VALUE r_isolate, VALUE value) -> VALUE {
          Isolate isolate(r_isolate);
          Locker lock(isolate);

          auto uint = v8::Integer::NewFromUnsigned(isolate, NUM2UINT(value));
          return Value::handleToRubyObject(isolate, uint);
        }).

      defineMethod("Value", [] (VALUE self) -> VALUE {
          Integer integer(self);
          Locker lock(integer);

          return INT2NUM(integer->Value());
        }).

      store(&Integer::Class);

    ClassBuilder("Int32", Integer::Class).

      defineMethod("Value", [] (VALUE self) -> VALUE {
          Int32 int32(self);
          Locker lock(int32);

          return INT2NUM(int32->Value());
        }).
      store(&Int32::Class);

    ClassBuilder("Uint32", Integer::Class).
      defineMethod("Value", [] (VALUE self) -> VALUE {
          Uint32 uint32(self);
          Locker lock(uint32);

          return UINT2NUM(uint32->Value());
        }).
      store(&Uint32::Class);
  }
}
