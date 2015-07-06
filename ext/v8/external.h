// -*- mode: c++ -*-
#ifndef EXTERNAL_H
#define EXTERNAL_H

namespace rr {
  class External : Ref<v8::External> {
  public:

    static void Init();
    static VALUE New(VALUE self, VALUE isolate, VALUE object);
    static VALUE Value(VALUE self);

    inline External(VALUE value) : Ref<v8::External>(value) {}
    inline External(v8::Isolate* isolate, v8::Handle<v8::External> handle) :
      Ref<v8::External>(isolate, handle) {}

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
