// -*- mode: c++ -*-
#ifndef RR_UINT32
#define RR_UINT32

namespace rr {

  /**
   * Converts between Ruby `Number` and the C/C++ `uint32_t`.
   *
   * This allows you to easily pass in `uint32_t` values whenever a
   * Ruby VALUE is expected (such as a method call) E.g.
   *
   *   uint_32_t myInt = 5;
   *   rb_funcall(UInt32(myInt), rb_intern("to_s")); //=> <String "5">
   *
   * It also converts a Ruby `VALUE` into its corresponding
   * `uint32_t`:
   *
   *   uint_32_t myInt = UInt32(rb_eval_string("5")); //=> 5
   *
   * Like all `Equiv`s, it stores itself internally as a Ruby `VALUE`
   */
  class UInt32 : public Equiv {
  public:
    /**
     * Construct a UInt32 from a Ruby `VALUE`
     */
    UInt32(VALUE val) : Equiv(val) {}

    /**
     * Construct a UInt32 from a `uint32_t` by converting it into its
     * corresponding `VALUE`.
     */
    UInt32(uint32_t ui) : Equiv(UINT2NUM(ui)) {}

    /**
     * Coerce the Ruby `VALUE` into a `uint32_t`.
     */
    inline operator uint32_t() {
      return RTEST(value) ? NUM2UINT(value) : 0;
    }
  };

}

#endif
