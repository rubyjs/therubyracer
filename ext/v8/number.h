// -*- mode: c++ -*-
#ifndef NUMBER_H
#define NUMBER_H

namespace rr {
  class Number : public Ref<v8::Number> {
  public:
    Number(v8::Isolate* isolate, double value) :
      Ref<v8::Number>(isolate, v8::Number::New(isolate, value)) {}
    Number(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Number>(isolate, value.As<v8::Number>()) {}
    Number(VALUE self) :
      Ref<v8::Number>(self) {}

    static void Init();
  };

  class Integer : public Ref<v8::Integer> {
  public:
    Integer(v8::Isolate* isolate, int32_t value) :
      Ref<v8::Integer>(isolate, v8::Integer::New(isolate, value)) {}
    Integer(v8::Isolate* isolate, uint32_t value) :
      Ref<v8::Integer>(isolate, v8::Integer::NewFromUnsigned(isolate, value)) {}
    Integer(VALUE self) :
      Ref<v8::Integer>(self) {}
  };

  class Int32 : public Ref<v8::Int32> {
  public:
    Int32(VALUE self) :
      Ref<v8::Int32>(self) {}
    Int32(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Int32>(isolate, value.As<v8::Int32>()) {}
  };

  class Uint32 : public Ref<v8::Uint32> {
  public:
    Uint32(VALUE self) :
      Ref<v8::Uint32>(self) {}
    Uint32(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Uint32>(isolate, value.As<v8::Uint32>()) {}
  };
}


#endif /* NUMBER_H */
