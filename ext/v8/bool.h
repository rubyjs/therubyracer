// -*- mode: c++ -*-
#ifndef RR_BOOL
#define RR_BOOL

namespace rr {

  /**
   * Seemlessly convert between Ruby booleans and C/C++.
   *
   * The `Bool` equivalence lets you plop in a Ruby boolean anywhere
   * you might need a C++ boolean, and, by the same token, drop in a
   * C/C++ boolean anywhere you might need a Ruby boolean. E.g.
   *
   * // Ruby -> C/C++
   * if (Bool(Qtrue)) {
   *   //always executed
   * }
   *
   * // C/C++ -> Ruby
   * if (RTEST(Bool(true))) {
   *   //always executed
   * }
   */
  class Bool : public Equiv {
  public:
    /**
     * Construct a Bool from a Ruby VALUE
     */
    Bool(VALUE val) : Equiv(val) {}

    /**
     * Constructo a Bool from a C/C++ boo. It is immediately
     * converted into the corresponding;
     */
    Bool(bool b) : Equiv(b ? Qtrue : Qfalse) {}

    /**
     * Construct a Bool from JavaScript.
     */
    Bool(v8::Handle<v8::Boolean> b) : Equiv(b->Value() ? Qtrue : Qfalse) {}

    /**
     * Coerce this into a native C/C++ bool. Since it is stored as a
     * Ruby VALUE, this is just a simple RTEST.
     *
     *   bool b =  Bool(true); // true
     *   b = Bool(false); // false
     *   b = Bool(Qtrue); // true
     *   b = Bool(Qfalse); // false
     *   b = Bool(Qnil); // false
     *   b = Bool(rb_cObject); // true
     */
    inline operator bool() {
      return RTEST(value);
    }

    /**
     * Use when you need to convert v8::Maybe<bool> into a VALUE. E.g.
     *   return Bool::Maybe(object->ReturnsMaybeSomething());
     */
    typedef Maybe<bool, Bool> Maybe;
  };
}

#endif
