// -*- mode: c++ -*-
#ifndef EXTERNAL_H
#define EXTERNAL_H

namespace rr {
  class External : public Ref<v8::External> {
  public:

    static void Init();
    static VALUE New(VALUE self, VALUE isolate, VALUE object);
    static VALUE Value(VALUE self);

    inline External(VALUE value) : Ref<v8::External>(value) {}
    inline External(v8::Isolate* isolate, v8::Handle<v8::External> handle) :
      Ref<v8::External>(isolate, handle) {}
    inline External(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      External(isolate, v8::Handle<v8::External>::Cast<v8::Value>(value)) {}

    static inline v8::Local<v8::External> wrap(Isolate isolate, VALUE object) {
      // as long as this external is alive within JavaScript, it should not be
      // garbage collected by Ruby.
      isolate.retainObject(object);

      // create the external.
      Container* container = new Container(object);
      v8::Local<v8::External> external(v8::External::New(isolate, (void*)container));

      // next, we create a weak reference to this external so that we can be
      // notified when V8 is done with it. At that point, we can let Ruby know
      // that this external is done with it.
      v8::Global<v8::External>* global(new v8::Global<v8::External>(isolate, external));
      global->SetWeak<Container>(container, &release, v8::WeakCallbackType::kParameter);
      container->global = global;

      return External(isolate, external);
    }
    static inline VALUE unwrap(v8::Local<v8::External> external) {
      Container* container = (Container*)external->Value();
      return container->object;
    };

    static inline VALUE unwrap(v8::Local<v8::Value> external) {
      return unwrap(v8::Local<v8::External>::Cast<v8::Value>(external));
    }

    struct Container {
      Container(VALUE v) : object(v) {}

      v8::Global<v8::External>* global;
      VALUE object;
    };

    /**
     * Implements a v8::WeakCallbackInfo<Container>::Callback with all
     * of its idiosyncracies. It happens in two passes. In the first
     * pass, you are required to only reset the weak reference. In the
     * second pass, you can actually do your cleanup. In this case, we
     * schedule the referenced Ruby object to be released in the next
     * Ruby gc pass.
     */
    static void release(const v8::WeakCallbackInfo<Container>& info) {
      Container* container(info.GetParameter());
      if (info.IsFirstPass()) {
        container->global->Reset();
        info.SetSecondPassCallback(&release);
      } else {
        Isolate isolate(info.GetIsolate());
        isolate.scheduleReleaseObject(container->object);
        delete container;
      }
    }
  };
}

#endif /* EXTERNAL_H */
