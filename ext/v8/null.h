// -*- mode: c++ -*-
#ifndef RR_NULL_H
#define RR_NULL_H

namespace rr {
  class Null : public Ref<v8::Value> {
  public:
    static inline void Init() {
      ClassBuilder("Null", Primitive::Class).
        store(&Class);

      //implement V8::C::Null(isolate)
      rb_define_singleton_method(rb_eval_string("V8::C"), "Null", (VALUE (*)(...))&instance, 1);
    }
    Null(v8::Isolate* isolate, v8::Local<v8::Value> undefined) :
      Ref<v8::Value>(isolate, undefined) {}

    static VALUE instance(VALUE self, VALUE rb_isolate) {
      Isolate isolate(rb_isolate);
      Locker lock(isolate);
      return Null(isolate, v8::Null(isolate));
    }
  };
}

#endif /* RR_NULL_H */
