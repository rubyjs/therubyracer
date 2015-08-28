// -*- mode: c++ -*-
#ifndef RR_HANDLE_H
#define RR_HANDLE_H
#include "rr.h"

namespace rr {
  class Handle : public Ref<void> {
  public:
    struct Finalizer;
    inline Handle(VALUE value) : Ref<void>(value) {}
    inline Handle(v8::Isolate* isolate, v8::Local<void> data)
      : Ref<void>(isolate, data) {}

    static inline void Init() {
      ClassBuilder("Handle").
        defineMethod("__DefineFinalizer__", &__DefineFinalizer__).
        store(&Class);
    }
    static VALUE __DefineFinalizer__(VALUE self, VALUE code) {
      Handle handle(self);
      v8::Isolate* isolate(handle);
      new Finalizer(isolate, handle, code);
      return Qnil;
    }

    /**
     * Finalizer is responsible for capturing a piece of Ruby code and
     * pushing it onto a queue once the V8 object points to is garbage
     * collected. It is passed a handle and a Ruby object at which
     * point it allocates a new storage cell that it holds
     * weakly. Once the object referenced by its storage cell is
     * garbage collected, the Finalizer enqueues the Ruby code so that
     * it can be run later from Ruby.
     */
    struct Finalizer {
      Finalizer(Isolate isolate, v8::Local<void> handle, VALUE code) :
        cell(new v8::Global<void>(isolate, handle)), callback(code) {

        // make sure that this code does not get GC'd by Ruby.
        isolate.retainObject(code);

        // install the callback
        cell->SetWeak<Finalizer>(this, &finalize, v8::WeakCallbackType::kParameter);
      }

      /**
       * When this finalizer container is destroyed, also clear out
       * the V8 storage cell and delete it.
       */
      inline ~Finalizer() {
        cell->Reset();
        delete cell;
      }

      /**
       * This implements a V8 GC WeakCallback, which will be invoked
       * whenever the given object is garbage collected. It's job is to
       * notify the Ruby isolate that the Ruby finalizer is ready to be
       * run, as well as to clean up the
       */
      static void finalize(const v8::WeakCallbackInfo<Finalizer>& info) {
        Isolate isolate(info.GetIsolate());
        Finalizer* finalizer(info.GetParameter());
        isolate.v8FinalizerReady(finalizer->callback);
        delete finalizer;
      }

      /**
       * The storage cell that is held weakly.
       */
      v8::Global<void>* cell;

      /**
       * The Ruby callable representing this finalizer.
       */
      VALUE callback;
    };
  };
}

#endif /* RR_HANDLE_H */
