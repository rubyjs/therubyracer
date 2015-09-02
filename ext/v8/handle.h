// -*- mode: c++ -*-
#ifndef RR_HANDLE_H
#define RR_HANDLE_H
#include "rr.h"

namespace rr {
  class Handle : public Ref<void> {
  public:
    struct Finalizer;
    inline Handle(VALUE value) : Ref<void>(value) {}

    static inline void Init() {
      ClassBuilder("Handle").
        defineSingletonMethod("New", &New).
        defineMethod("IsEmpty", &IsEmpty).
        defineMethod("SetWeak", &SetWeak).
        defineMethod("ClearWeak", &ClearWeak).
        store(&Class);
    }

    /**
     * Creates a New Handle to this object of the same class. If you
     * have a V8::C::Object, then V8::C::Handle::New(object) will also
     * be a V8::C::Object and represent a completely new handle to the
     * object that will also prevent the object from being garbage
     * collected by v8 (provided it has not be Reset())
     */
    static VALUE New(VALUE self, VALUE other) {
      if (!rb_funcall(other, rb_intern("kind_of?"), 1, Handle::Class)) {
        rb_raise(rb_eArgError, "not a V8::C::Handle");
        return Qnil;
      } else {
        VALUE cls = rb_class_of(other);
        Ref<void> ref(other);
        v8::Isolate* isolate(ref);
        v8::Local<void> handle(ref);
        return Data_Wrap_Struct(cls, 0, &destroy, new Holder(isolate, handle));
      }
    }

    /**
     * Calls v8::Handle::SetWeak, but the API is slightly different
     * than the C++. The only parameter is a callable object that will
     * be enqueued when value referenced by this handle is garbage
     * collected. This code will not be called immediately. Instead,
     * each callable must be iterated through from within Ruby code
     * using the Isolate#__EachV8Finalizer__ method. Which will
     * dequeue all finalizers that have not been yet run and yield
     * them to the passed block. The sequence is roughly this:
     *
     * 1. value becomes finalizable
     * 2. the v8 native finalizer runs.
     * 3. Ruby callable is enqueued and will be seen by the next
     *    invocation of __EachV8Finalizer__
     */
    static VALUE SetWeak(VALUE self, VALUE callback) {
      Handle handle(self);
      Isolate isolate((v8::Isolate*)handle);

      // make sure this callback is not garbage collected
      isolate.retainObject(callback);

      Holder* holder(handle.unwrapHolder());
      Finalizer* finalizer = new Finalizer(holder->cell, callback);

      // mark weak and install the callback
      holder->cell->SetWeak<Finalizer>(finalizer, &finalize, v8::WeakCallbackType::kParameter);
      return Qnil;
    }

    static VALUE ClearWeak(VALUE self) {
      Handle handle(self);
      Locker lock(handle);

      Holder* holder(handle.unwrapHolder());

      Finalizer* finalizer = holder->cell->ClearWeak<Finalizer>();
      delete finalizer;
      return Qnil;
    }

    static VALUE IsEmpty(VALUE self) {
      Handle handle(self);
      Locker lock(handle);

      Holder* holder(handle.unwrapHolder());
      return Bool(holder->cell->IsEmpty());
    }

    static void finalize(const v8::WeakCallbackInfo<Finalizer>& info) {
      Isolate isolate(info.GetIsolate());
      Finalizer* finalizer = info.GetParameter();

      // clear the storage cell. This is required by the V8 API.
      finalizer->cell->Reset();

      // notify that this finalizer is ready to run.
      isolate.v8FinalizerReady(finalizer->callback);
      delete finalizer;
    }

    /**
     * A simple data structure to hold the objects necessary for
     * finalization.
     */
    struct Finalizer {
      Finalizer(v8::Persistent<void>* cell_, VALUE code) :
        cell(cell_), callback(code) {
      }

      /**
       * The storage cell that is held weakly.
       */
      v8::Persistent<void>* cell;

      /**
       * The Ruby callable representing this finalizer.
       */
      VALUE callback;
    };
  };
}

#endif /* RR_HANDLE_H */
