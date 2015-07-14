// -*- mode: c++ -*-
#ifndef RR_INTEGER_H
#define RR_INTEGER_H

namespace rr {
  class Integer : public Ref<v8::Integer> {
  public:
    Integer(v8::Isolate* isolate, int32_t value) :
      Ref<v8::Integer>(isolate, v8::Integer::New(isolate, value)) {}
    Integer(v8::Isolate* isolate, uint32_t value) :
      Ref<v8::Integer>(isolate, v8::Integer::NewFromUnsigned(isolate, value)) {}
    Integer(VALUE self) :
      Ref<v8::Integer>(self) {}

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
