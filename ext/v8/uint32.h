// -*- mode: c++ -*-
#ifndef RR_UINT32_H
#define RR_UINT32_H


namespace rr {

  class Uint32 : public Ref<v8::Uint32> {
  public:
    Uint32(VALUE self) :
      Ref<v8::Uint32>(self) {}
    Uint32(v8::Isolate* isolate, v8::Handle<v8::Value> value) :
      Ref<v8::Uint32>(isolate, value.As<v8::Uint32>()) {}

    static VALUE Value(VALUE self) {
      Uint32 uint32(self);
      Locker lock(uint32);

      return UINT2NUM(uint32->Value());
    }

    static inline void Init() {
      ClassBuilder("Uint32", Integer::Class).
        defineMethod("Value", &Value).
        store(&Class);
    }
  };


//TODO: remove this at some point. I don't see what this provides
//above and beyond just using UINT2NUM and NUM2UINT.
// --cowboyd Jul 9, 2015

  /**
   * Converts between Ruby `Number` and the C/C++ `uint32_t`.
   *
   * This allows you to easily pass in `uint32_t` values whenever a
   * Ruby VALUE is expected (such as a method call) E.g.
   *
   *   uint_32_t myInt = 5;
   *   rb_funcall(Uint32_t(myInt), rb_intern("to_s")); //=> <String "5">
   *
   * It also converts a Ruby `VALUE` into its corresponding
   * `uint32_t`:
   *
   *   uint_32_t myInt = Uint32_t(rb_eval_string("5")); //=> 5
   *
   * Like all `Equiv`s, it stores itself internally as a Ruby `VALUE`
   */
  class Uint32_t : public Equiv {
  public:
    /**
     * Construct a Uint32_t from a Ruby `VALUE`
     */
    Uint32_t(VALUE val) : Equiv(val) {}

    /**
     * Construct a Uint32_t from a `uint32_t` by converting it into its
     * corresponding `VALUE`.
     */
    Uint32_t(uint32_t ui) : Equiv(UINT2NUM(ui)) {}

    /**
     * Coerce the Ruby `VALUE` into a `uint32_t`.
     */
    inline operator uint32_t() {
      return RTEST(value) ? NUM2UINT(value) : 0;
    }
  };

}

#endif
