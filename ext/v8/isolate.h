// -*- mode: c++ -*-
#ifndef RR_ISOLATE
#define RR_ISOLATE

#include "vendor/concurrentqueue.h"

using namespace moodycamel;

namespace rr {
  /**
   * V8::C::Isolate
   *
   * Represents a fully encapsulated V8 virtual machine. Allocated
   * from Ruby by calling `V8::C::Isolate::New()`
   *
   * Every v8::Isolate wrapped in Ruby will have an instance of
   * `IsolateData` embedded in it that can be used for bookkeeping
   * between the V8 and Ruby worlds. For example, when v8 objects are
   * no longer needed by ruby, they'll be enqueued for later release
   * inside the V8 garbarge collection thread. This queue lives in the
   * `IsolateData`
   *
   * Note: You must call `Dispose()` on the isolate for its resources
   * to be released, otherwise, it will be leaked.
   */
  class Isolate  {
  public:
    struct IsolateData;
    static VALUE Class;
    static void Init();

    static VALUE New(VALUE self);
    static VALUE SetCaptureStackTraceForUncaughtExceptions(VALUE self, VALUE capture, VALUE stack_limit, VALUE options);
    static VALUE ThrowException(VALUE self, VALUE error);
    static VALUE IdleNotificationDeadline(VALUE self, VALUE deadline_in_seconds);
    static VALUE RequestGarbageCollectionForTesting(VALUE self);
    static VALUE __EachV8Finalizer__(VALUE self);

    inline Isolate(IsolateData* data_) : data(data_) {}
    inline Isolate(v8::Isolate* isolate) :
      data((IsolateData*)isolate->GetData(0)) {}
    inline Isolate(VALUE value) {
      Data_Get_Struct(value, struct IsolateData, data);
    }

    /**
     * Called, when Ruby no longer has any more references to this
     * instance of `V8::C::Isolate`. However, this does not
     * necessarily mean that we can delete the isolate data because there
     * could be outstanding objects... things like a V8::C::String
     * that are part of this isolate that have yet to be garbage
     * collected. The last object in the isolate, including the
     * isolate itself is responsible for deleting the isolate data.
     *
     */
    static void destroy(IsolateData* data) {
      Isolate isolate(data);
      isolate->Dispose();
      isolate.decrementTotalReferences();
    }

    /**
     * Every time we take out a reference to a V8 object, call this
     * method.
     */
    inline void incrementTotalReferences() {
      char counter(0);
      data->total_ruby_references_queue.enqueue(counter);
    }

    /**
     * Every time a Ruby reference to a V8 object is garbage
     * collected, call this method. If this is the last object
     * associated with this isolate, then the isolate data can, and
     * will, be safely deleted.
     */
    inline bool decrementTotalReferences() {
      char counter(0);
      if (data->total_ruby_references_queue.try_dequeue(counter)) {
        return true;
      } else {
        delete data;
        return false;
      }
    }

    /**
     * Converts the v8::Isolate into a Ruby Object, while setting up
     * its book keeping data. E.g.
     * VALUE rubyObject = Isolate(v8::Isolate::New());
     */
    virtual operator VALUE() {
      return Data_Wrap_Struct(Class, &releaseAndMarkRetainedObjects, &destroy, data);
    }

    /**
     * Convert this into a v8::Isolate* for those areas of the API
     * that call for it: E.g.
     *
     *   v8::Context::New(Isolate(self));
     */
    inline operator v8::Isolate*() { return data->isolate; }

    /**
     * Dereference the underlying v8::Isolate so that we can call methods
     * on it. E.g.
     *
     *   Isolate(self)->IsDead();
     */

    inline v8::Isolate* operator ->() { return data->isolate; }

    /**
     * Schedule a v8::Persistent reference to be be deleted with the next
     * invocation of the V8 Garbarge Collector. It is safe to call
     * this method from within the Ruby garbage collection thread or a place
     * where you do not want to acquire any V8 locks.
     */
    template <class T>
    inline void scheduleReleaseObject(v8::Persistent<T>* cell) {
      if (this->decrementTotalReferences()) {
        data->v8_release_queue.enqueue((v8::Persistent<void>*)cell);
      }
    }

    /**
     * Schedule a Ruby object to be released with the next invocation
     * of the Ruby garbage collector. This method is safe to call from places
     * where you do not hold any Ruby locks (such as the V8 GC thread)
     */
    inline void scheduleReleaseObject(VALUE object) {
      data->rb_release_queue.enqueue(object);
    }

    /**
     * Increase the reference count to this Ruby object by one. As
     * long as there is more than 1 reference to this object, it will
     * not be garbage collected, even if there are no references to
     * from within Ruby code.
     *
     * Note: should be called from a place where Ruby locks are held.
     */
    inline void retainObject(VALUE object) {
      rb_funcall(data->retained_objects, rb_intern("add"), 1, object);
    }

    /**
     * Decrease the reference count to this Ruby object by one. If the
     * count falls below zero, this object will no longer be marked my
     * this Isolate and will be eligible for garbage collection.
     *
     * Note: should be called from a place where Ruby locks are held.
     */
    inline void releaseObject(VALUE object) {
      rb_funcall(data->retained_objects, rb_intern("remove"), 1, object);
    }

    /**
     * Indicate that a finalizer that had been associated with a given
     * V8 object is now ready to run because that V8 object has now
     * been garbage collected.
     *
     * This can be called from anywhere and does not need to hold
     * either Ruby or V8 locks. It is designed though to be called
     * from the V8 GC callback that determines that the object is no
     * more.
     */
    inline void v8FinalizerReady(VALUE code) {
      data->v8_finalizer_queue.enqueue(code);
    }

    /**
     * Iterates through all of the V8 finalizers that have been marked
     * as ready and yields them. They wil be dequeued after this
     * point, and so will never be seen again.
     */
    inline void eachV8Finalizer(int* state) {
      VALUE finalizer;
      while (data->v8_finalizer_queue.try_dequeue(finalizer)) {
        rb_protect(&yieldOneV8Finalizer, finalizer, state);
        // we no longer need to retain this object from garbage
        // collection.
        releaseObject(finalizer);
        if (*state != 0) {
          break;
        }
      }
    }
    /**
     * Yield a single value. This is wrapped in a function, so that
     * any exceptions that happen don't blow out the stack.
     */
    static VALUE yieldOneV8Finalizer(VALUE finalizer) {
      return rb_yield(finalizer);
    }

    /**
     * The `gc_mark()` callback for this Isolate's
     * Data_Wrap_Struct. It releases all pending Ruby objects.
     */
    static void releaseAndMarkRetainedObjects(IsolateData* data) {
      Isolate isolate(data);
      VALUE object;
      while (data->rb_release_queue.try_dequeue(object)) {
        isolate.releaseObject(object);
      }
      //TODO: This should not be necessary since sometimes the
      //instance of V8::RetainedObjects appears to magically be of
      //type T_NONE instead of T_OBJECT. Later, it will be T_OBJECT,
      //but if called while T_NONE, it will cause rb_gc_mark to dump
      //core.
      //See https://bugs.ruby-lang.org/issues/10803
      if (TYPE(data->retained_objects) != T_NONE) {
        rb_gc_mark(data->retained_objects);
      }
    }

    /**
     * An instance of v8::GCPrologueCallback, this will run in the v8
     * GC thread, and clear out all the references that have been
     * released from Ruby.
     */
    static void clearReferences(v8::Isolate* i, v8::GCType type, v8::GCCallbackFlags flags) {
      Isolate isolate(i);
      v8::Persistent<void>* cell;
      while (isolate.data->v8_release_queue.try_dequeue(cell)) {
        cell->Reset();
        delete cell;
      }
    }

    /**
     * Recent versions of V8 will segfault unless you pass in an
     * ArrayBufferAllocator into the create params of an isolate. This
     * is the simplest implementation possible.
     */
    class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
    public:
      virtual void* Allocate(size_t length) {
        void* data = AllocateUninitialized(length);
        return data == NULL ? data : memset(data, 0, length);
      }
      virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
      virtual void Free(void* data, size_t) { free(data); }
    };

    /**
     * Data specific to the Ruby embedding. It has the same life span
     * as the isolate.
     */
    struct IsolateData {

      /**
       * The actual Isolate
       */
      v8::Isolate* isolate;

      /**
       * An instance of `V8::RetainedObjects` that contains all
       * references held from from V8. As long as they are in this
       * list, they won't be gc'd by Ruby.
       */
      VALUE retained_objects;

      /**
       * A custom ArrayBufferAllocator for this isolate. Why? because
       * if you don't, it will segfault when you try and create a
       * Context. That's why.
       */
      ArrayBufferAllocator array_buffer_allocator;

      /**
       * Queue to hold unused references to v8 objects. Once Ruby is
       * finished with an object it will be enqueued here so that it
       * can be released by the v8 garbarge collector later.
       */
      ConcurrentQueue<v8::Persistent<void>*> v8_release_queue;

      /**
       * Ruby objects that had been retained by this isolate, but that
       * are eligible for release. Generally, an object ends up in a
       * queue when the v8 object that had referenced it no longer
       * needs it.
       */
      ConcurrentQueue<VALUE> rb_release_queue;

      /**
       * Sometimes it is useful to get a callback into Ruby whenever a
       * JavaScript object is garbage collected by V8. This is done by
       * calling v8_object._DefineFinalizer(some_proc). However, we
       * cannot actually run this Ruby code inside the V8 garbage
       * collector. It's not safe! It might end up allocating V8
       * objects, or doing all kinds of who knows what! Instead, the
       * ruby finalizer gets pushed onto this queue where it can be
       * invoked later from ruby code with a call to
       * isolate.__RunV8Finalizers__!
       */
      ConcurrentQueue<VALUE> v8_finalizer_queue;

      /**
       * Contains a number of tokens representing all of the live Ruby
       * references that are currently active in this Isolate. Every
       * time a Ruby reference to a V8 object is added, an item is
       * added to this queue. Every time a ruby reference is garbage
       * collected, an item is pulled out of the queue. Whenever this
       * queue reaches 0, then it is time to delete the isolate data
       * since there are no more object associated with it.
       */
      ConcurrentQueue<int> total_ruby_references_queue;

    };

    IsolateData* data;
  };
}

#endif
