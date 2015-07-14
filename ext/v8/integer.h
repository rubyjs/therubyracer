// -*- mode: c++ -*-
#ifndef RR_INTEGER_H
#define RR_INTEGER_H

namespace rr {
  class Integer : public Ref<v8::Integer> {
  public:
    Integer(v8::Isolate* isolate, v8::Handle<v8::Integer> integer) :
      Ref<v8::Integer>(isolate, integer) {}
    Integer(VALUE self) :
      Ref<v8::Integer>(self) {}
    class Maybe : MaybeLocal<v8::Integer, Integer> {};

    static VALUE New(VALUE self, VALUE r_isolate, VALUE value) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      v8::Local<v8::Integer> i = v8::Integer::New(isolate, NUM2INT(value));
      return Value::handleToRubyObject(isolate, i);
    }

    static VALUE NewFromUnsigned(VALUE self, VALUE r_isolate, VALUE value) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      uint32_t uint = NUM2UINT(value);
      v8::Local<v8::Integer> i = v8::Integer::NewFromUnsigned(isolate, uint);
      return Value::handleToRubyObject(isolate, i);
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
    }
  };
}

#endif /* RR_INTEGER_H */
