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
   *
   * It is handy to have a class to do the conversions instead of
   * preprocessor macros like `NUM2INT`, et al. because classes can be
   * easily used in C++ templates.
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


    /**
     * A Maybe class for primitive conversions. Specify the primitive
     * type and the covversion type, and it will be wrapped in a
     * Maybe. E.g.
     *
     *   v8::Maybe<bool> maybe = methodReturningMaybeBool();
     *   return Equiv::Maybe<bool, Bool>(maybe);
     *
     * will yield a V8::C::Maybe wrapping the underlying maybe value.
     */
    template <class T, class S>
    class Maybe : public rr::Maybe {
    public:
      Maybe(v8::Maybe<T> maybe) {
        if (maybe.IsJust()) {
          just(S(maybe.FromJust()));
        }
      }
    };

  protected:
    VALUE value;
  };

}

#endif
