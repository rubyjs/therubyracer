// -*- mode: c++ -*-
#ifndef RR_SIGNATURE_H
#define RR_SIGNATURE_H

namespace rr {
  class Signature : public Ref<v8::Signature> {
  public:
    Signature(VALUE self) : Ref<v8::Signature>(self) {}
    Signature(v8::Isolate* i, v8::Local<v8::Signature> s) :
      Ref<v8::Signature>(i, s) {}

    static void Init() {
      ClassBuilder("Signature").
        defineSingletonMethod("New", &New).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self);
  };
}

#endif /* RR_SIGNATURE_H */
