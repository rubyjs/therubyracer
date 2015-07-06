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

    static void clearDeadReferences(v8::Isolate* i, v8::GCType type, v8::GCCallbackFlags flags) {
      Isolate isolate(i);
      v8::Persistent<void>* cell;
      while (isolate.data()->queue.try_dequeue(cell)) {
        cell->Reset();
        delete cell;
      }
    }

    /**
     * Converts the v8::Isolate into a Ruby Object, while setting up
     * its book keeping data. E.g.
     * VALUE rubyObject = Isolate(v8::Isolate::New());
     */
    inline operator VALUE() {
      return Data_Wrap_Struct(Class, 0, 0, pointer);
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
     * invocation of the V8 Garbarge Collector
     */
    template <class T>
    inline void scheduleDelete(v8::Persistent<T>* cell) {
      data()->queue.enqueue((v8::Persistent<void>*)cell);
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
      ConcurrentQueue<v8::Persistent<void>*> queue;
    };
  };
}

#endif
