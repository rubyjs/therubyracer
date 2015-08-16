// -*- mode: c++ -*-
#ifndef RR_ACCESSOR_SIGNATURE_H
#define RR_ACCESSOR_SIGNATURE_H

namespace rr {
  class AccessorSignature : public Ref<v8::AccessorSignature> {
  public:
    AccessorSignature(VALUE self) : Ref<v8::AccessorSignature>(self) {}
    AccessorSignature(v8::Isolate* i, v8::Local<v8::AccessorSignature> s) :
      Ref<v8::AccessorSignature>(i, s) {}

    static void Init() {
      ClassBuilder("AccessorSignature").
        defineSingletonMethod("New", &New).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self);
  };
}

#endif /* RR_ACCESSOR_SIGNATURE_H */
