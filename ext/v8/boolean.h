// -*- mode: c++ -*-

#ifndef RR_BOOLEAN_H
#define RR_BOOLEAN_H

namespace rr {
  class Boolean : public Ref<v8::Boolean> {
  public:
    Boolean(v8::Isolate* isolate, v8::Local<v8::Boolean> handle) :
      Ref<v8::Boolean>(isolate, handle) {}
    Boolean(VALUE value) : Ref<v8::Boolean>(value) {}

    static void Init() {
      ClassBuilder("Boolean", Value::Class).
        defineSingletonMethod("New", &New).
        defineMethod("Value", &Value).
        store(&Class);
    }

    static VALUE New(VALUE self, VALUE r_isolate, VALUE boolean) {
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      return Boolean(isolate, v8::Boolean::New(isolate, RTEST(boolean)));
    }

    static VALUE Value(VALUE self) {
      Boolean boolean(self);
      Locker lock(boolean);

      if (boolean->Value()) {
        return Qtrue;
      } else {
        return Qfalse;
      }
    }
  };
}

#endif /* RR_BOOLEAN_H */
