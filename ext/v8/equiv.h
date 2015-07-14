// -*- mode: c++ -*-
#ifndef RR_EQUIV
#define RR_EQUIV

namespace rr {

  /**
   * Defines an equivalence between a primitive Ruby and C/C++ types so
   * that they can be used inter-changeably. In other words: given a
   * Ruby `VALUE` it can convert it to a C/C++ type, and given a C/C++
   * type, it can convert it to a Ruby type. For example, the Boolean
   * equivalence class defines the relationship between Ruby booleans
   * and C/++ booleans, and can be used to place a Ruby VALUE anywhere
   * you might need a C/C++ boolean such as an "if" statement:
   *
   *   if (rr::Bool(Qnil)) { //
   *    throw "not going to happen!";
   *   }
   *
   * This is the superclass of all equivalences classes,
   * and is not meant to be instantiated.
   *
   * Internally, `Equiv`s are always stored as a Ruby `VALUE`, and so
   * part of the job of the subclass is to have an appropriate
   * constructor that converts the C/C++ type to a `VALUE`
   */
  class Equiv {
  public:
    /**
     * Constructs an `Equiv` from a Ruby VALUE. It's up to the
     * subclass to determine what it can be converted to.
     */
    Equiv(VALUE val) : value(val) {}

    /**
     * Converts this `Equiv` into a Ruby `VALUE`. This, of course, is
     * just the value of the internal storage. In the Boolean example:
     *
     * VALUE val = Bool(Qnil); //=> Qnil;
     */
    inline operator VALUE() { return value; }

  protected:
    VALUE value;
  };

  /**
   * Convert `int`s into `VALUE`. To unbox v8::Maybe<int>, use
   * Int::Maybe().
   */
  class Int : public Equiv {
    Int(int i) : Equiv(INT2FIX(i)) {}

    class Maybe : rr::Maybe<int, Int> {};
  };
}

#endif
