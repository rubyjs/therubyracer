// -*- mode: c++ -*-
#ifndef RR_ISOLATE
#define RR_ISOLATE

namespace rr {
  /**
   * V8::C::Isolate
   *
   * Represents a fully encapsulated V8 virtual machine. Allocated
   * from Ruby by calling `V8::C::Isolate::New()`
   *
   * Note: You must call `Dispose()` on the isolate for its resources
   * to be released.
   */
  class Isolate : public Pointer<v8::Isolate> {
  public:
    static void Init();

    static VALUE New(VALUE self);

    inline Isolate(v8::Isolate* isolate) : Pointer<v8::Isolate>(isolate) {}
    inline Isolate(VALUE value) : Pointer<v8::Isolate>(value) {}

    inline operator VALUE() {
      return Data_Wrap_Struct(Class, 0, 0, pointer);
    }

    static VALUE Dispose(VALUE self);
  };
}

#endif
