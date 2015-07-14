// -*- mode: c++ -*-
#ifndef RR_NUMBER_H
#define RR_NUMBER_H

namespace rr {
  class Number : public Ref<v8::Number> {
  public:
    Number(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Number>(isolate, value.As<v8::Number>()) {}
    Number(VALUE self) :
      Ref<v8::Number>(self) {}

    static VALUE New(VALUE self, VALUE r_isolate, VALUE value) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      return Number(isolate, v8::Number::New(isolate, NUM2DBL(value)));
    }

    static VALUE Value(VALUE self) {
      Number number(self);
      Locker lock(number);

      return DBL2NUM(number->Value());
    }
    static void Init() {
      ClassBuilder("Number", Primitive::Class).
        defineSingletonMethod("New", &New).
        defineMethod("Value", &Value).
        store(&Class);
    }
  };
}


#endif /* RR_NUMBER_H */
