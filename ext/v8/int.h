// -*- mode: c++ -*-
#ifndef RR_INT_H
#define RR_INT_H


namespace rr {
  /**
   * Converts between Ruby `Number` and the C/C++ `int`.
   *
   * This allows you to easily pass in `int` values whenever a
   * Ruby VALUE is expected (such as a method call) E.g.
   *
   *   int myInt = 5;
   *   rb_funcall(Uint32_t(myInt), rb_intern("to_s")); //=> <String "5">
   *
   * It also converts a Ruby `VALUE` into its corresponding
   * `int`:
   *
   *   intt myInt = Int(rb_eval_string("5")); //=> 5
   *
   * Like all `Equiv`s, it stores itself internally as a Ruby `VALUE`
   */
  class Int : public Equiv {
  public:
    /**
     * Use to convert methods that return Maybe<int> to a Ruby
     * VALUE
     *
     *   return Int::Maybe(stack_trace->GetStartColumn(context));
     */
    typedef Equiv::Maybe<int, Int> Maybe;

    /**
     * Construct an Int from a Ruby `VALUE`
     */
    Int(VALUE val) : Equiv(val) {}

    /**
     * Construct an Int from a `int` by converting it into its
     * corresponding `VALUE`.
     */
    Int(int i) : Equiv(INT2FIX(i)) {}

    /**
     * Coerce the Ruby `VALUE` into an `int`.
     */
    inline operator int() {
      return RTEST(value) ? NUM2UINT(value) : 0;
    }
  };

}

#endif
