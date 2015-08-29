// -*- mode: c++ -*-

#ifndef RR_FUNCTION_TEMPLATE_H
#define RR_FUNCTION_TEMPLATE_H

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
        defineMethod("SetCallHandler", &SetCallHandler).
        defineMethod("SetLength", &SetLength).
        defineMethod("InstanceTemplate", &InstanceTemplate).
        defineMethod("PrototypeTemplate", &PrototypeTemplate).
        defineMethod("SetClassName", &SetClassName).
        defineMethod("SetAcceptAnyReceiver", &SetAcceptAnyReceiver).
        defineMethod("SetHiddenPrototype", &SetHiddenPrototype).
        defineMethod("ReadOnlyPrototype", &ReadOnlyPrototype).
        defineMethod("RemovePrototype", &RemovePrototype).
        defineMethod("HasInstance", &HasInstance).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, r_callback, r_data, r_signature, r_length;
      rb_scan_args(argc, argv, "14", &r_isolate, &r_callback, &r_data, &r_signature, &r_length);
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      FunctionCallback callback(isolate, r_callback, r_data);
      Signature signature(r_signature);

      return FunctionTemplate(isolate, v8::FunctionTemplate::New(isolate, callback, callback, v8::Local<v8::Signature>(), Int(r_length)));
    }

    static VALUE GetFunction(VALUE self, VALUE context) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return Function::Maybe(isolate, t->GetFunction(Context(context)));
    }

    static VALUE SetCallHandler(VALUE self, VALUE r_callback, VALUE r_data) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);
      FunctionCallback callback(isolate, r_callback, r_data);

      t->SetCallHandler(callback, callback);

      return Qnil;
    }

    static VALUE SetLength(VALUE self, VALUE r_length) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->SetLength(Int(r_length));

      return Qnil;
    }

    static VALUE InstanceTemplate(VALUE self) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return ObjectTemplate(isolate, t->InstanceTemplate());
    }

    static VALUE PrototypeTemplate(VALUE self) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return ObjectTemplate(isolate, t->PrototypeTemplate());
    }

    static VALUE SetClassName(VALUE self, VALUE name) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->SetClassName(String(name));

      return Qnil;
    }

    static VALUE SetAcceptAnyReceiver(VALUE self, VALUE value) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->SetAcceptAnyReceiver(Bool(value));

      return Qnil;
    }

    static VALUE SetHiddenPrototype(VALUE self, VALUE value) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->SetHiddenPrototype(Bool(value));

      return Qnil;
    }

    static VALUE ReadOnlyPrototype(VALUE self) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->ReadOnlyPrototype();

      return Qnil;
    }

    static VALUE RemovePrototype(VALUE self) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      t->ReadOnlyPrototype();

      return Qnil;
    }

    static VALUE HasInstance(VALUE self, VALUE object) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return Bool(t->HasInstance(Value(object)));
    }
  };
}

#endif /* RR_FUNCTION_TEMPLATE_H */
