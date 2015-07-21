// -*- mode: c++ -*-
#ifndef RR_UNDEFINED_H
#define RR_UNDEFINED_H

namespace rr {
  class Undefined : public Ref<v8::Value> {
  public:
    Undefined(v8::Isolate* isolate, v8::Local<v8::Value> undefined) :
      Ref<v8::Value>(isolate, undefined) {}

    static inline void Init() {
      ClassBuilder("Undefined", Primitive::Class).
        store(&Class);
    }
  };
}

#endif /* RR_UNDEFINED_H */
