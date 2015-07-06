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
  class Isolate : public Pointer<v8::Isolate> {
  public:
    struct IsolateData;
    static void Init();

    static VALUE New(VALUE self);

    inline Isolate(v8::Isolate* isolate) : Pointer<v8::Isolate>(isolate) {}
    inline Isolate(VALUE value) : Pointer<v8::Isolate>(value) {}

    /**
     * Converts the v8::Isolate into a Ruby Object, while setting up
     * its book keeping data. E.g.
     * VALUE rubyObject = Isolate(v8::Isolate::New());
     */
    virtual operator VALUE() {
      return Data_Wrap_Struct(Class, &releaseAndMarkRetainedObjects, 0, pointer);
    }

    /**
     * Access the book-keeping data. e.g.
     *
     *   Isolate(self).data();
     */
    inline IsolateData* data() {
      return (IsolateData*)pointer->GetData(0);
    }

    /**
     * Schedule a v8::Persistent reference to be be deleted with the next
     * invocation of the V8 Garbarge Collector. It is safe to call
     * this method from within the Ruby garbage collection thread or a place
     * where you do not want to acquire any V8 locks.
     */
    template <class T>
    inline void scheduleReleaseObject(v8::Persistent<T>* cell) {
      data()->v8_release_queue.enqueue((v8::Persistent<void>*)cell);
    }

    /**
     * Schedule a Ruby object to be released with the next invocation
     * of the Ruby garbage collector. This method is safe to call from places
     * where you do not hold any Ruby locks (such as the V8 GC thread)
     */
    inline void scheduleReleaseObject(VALUE object) {
      data()->rb_release_queue.enqueue(object);
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
      rb_funcall(data()->retained_objects, rb_intern("add"), 1, object);
    }

    /**
     * Decrease the reference count to this Ruby object by one. If the
     * count falls below zero, this object will no longer be marked my
     * this Isolate and will be eligible for garbage collection.
     *
     * Note: should be called from a place where Ruby locks are held.
     */
    inline void releaseObject(VALUE object) {
      rb_funcall(data()->retained_objects, rb_intern("remove"), 1, object);
    }


    /**
     * The `gc_mark()` callback for this Isolate's
     * Data_Wrap_Struct. It releases all pending Ruby objects.
     */

    static void releaseAndMarkRetainedObjects(v8::Isolate* isolate_) {
      Isolate isolate(isolate_);
      IsolateData* data = isolate.data();
      VALUE object;
      while (data->rb_release_queue.try_dequeue(object)) {
        isolate.releaseObject(object);
      }
      rb_gc_mark(data->retained_objects);
    }

    /**
     * An instance of v8::GCPrologueCallback, this will run in the v8
     * GC thread, and clear out all the references that have been
     * released from Ruby.
     */
    static void clearReferences(v8::Isolate* i, v8::GCType type, v8::GCCallbackFlags flags) {
      Isolate isolate(i);
      v8::Persistent<void>* cell;
      while (isolate.data()->v8_release_queue.try_dequeue(cell)) {
        cell->Reset();
        delete cell;
      }
    }


    static VALUE Dispose(VALUE self);

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
       * An instance of `V8::RetainedObjects` that contains all
       * references held from from V8. As long as they are in this
       * list, they won't be gc'd by Ruby.
       */
      VALUE retained_objects;

      /**
       * A custom ArrayBufferAllocator for this isolate. Why? because
       * if you don't, it will segfault when you try and create an
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
    };
  };
}

#endif
