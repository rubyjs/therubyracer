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
    inline operator VALUE() {
      pointer->SetData(0, new IsolateData());
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

    inline void scheduleDelete(v8::Global<void>* cell) {
      data()->queue.enqueue(cell);
    }

    static VALUE Dispose(VALUE self);

    struct IsolateData {
      ConcurrentQueue<v8::Global<void>*> queue;
    };
  };
}

#endif
