// -*- mode: c++ -*-

namespace rr {
  class FunctionTemplate : public Ref<v8::FunctionTemplate> {
  public:
    FunctionTemplate(VALUE self) : Ref<v8::FunctionTemplate>(self) {}
    FunctionTemplate(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> t) :
      Ref<v8::FunctionTemplate>(isolate, t) {}
    static inline void Init() {
      ClassBuilder("FunctionTemplate", Template::Class).
        defineSingletonMethod("New", &New).
        defineMethod("GetFunction", &GetFunction).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, callback, data, signature, length;
      rb_scan_args(argc, argv, "14", &r_isolate, &callback, &data, &signature, &length);
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      return FunctionTemplate(isolate, v8::FunctionTemplate::New(isolate));
    }

    static VALUE GetFunction(VALUE self, VALUE context) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return Function::Maybe(isolate, t->GetFunction(Context(context)));
    }
  };
}
