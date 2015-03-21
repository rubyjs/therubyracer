#ifndef RR_ISOLATE
#define RR_ISOLATE

namespace rr {

  class Isolate : public Pointer<v8::Isolate> {
  public:
    static void Init();

    static VALUE New(VALUE self);
    static VALUE Enter(VALUE self);
    static VALUE Exit(VALUE self);
    static VALUE GetCurrent(VALUE self);

    // TODO: Add a Dispose method

    inline Isolate(v8::Isolate* isolate) : Pointer<v8::Isolate>(isolate) {}
    inline Isolate(VALUE value) : Pointer<v8::Isolate>(value) {}

    inline operator VALUE() {
      return Data_Wrap_Struct(Class, 0, &release, pointer);
    }

    static void release(v8::Isolate* isolate) {
      // The isolates must be released with Dispose.
      // Using the delete operator is not allowed.

      // TODO: Do we want to dispose of the isolate when the object itself
      // is garbage-collected?
      // Can the isolate be used without it having a reference in ruby world?
      // isolate->Dispose();
    }
  };

}

#endif
