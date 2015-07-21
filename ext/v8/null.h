// -*- mode: c++ -*-
#ifndef RR_NULL_H
#define RR_NULL_H

namespace rr {
  class Null : public Ref<v8::Value> {
  public:
    Null(v8::Isolate* isolate, v8::Local<v8::Value> undefined) :
      Ref<v8::Value>(isolate, undefined) {}

    static inline void Init() {
      ClassBuilder("Null", Primitive::Class).
        store(&Class);
    }
  };
}

#endif /* RR_NULL_H */
