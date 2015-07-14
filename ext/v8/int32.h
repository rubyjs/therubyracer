// -*- mode: c++ -*-
#ifndef RR_INT32_H
#define RR_INT32_H

namespace rr {
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

    static void Init() {
      ClassBuilder("Int32", Integer::Class).
        defineMethod("Value", &Value).
        store(&Int32::Class);
    }
  };

}

#endif /* RR_INT32_H */
