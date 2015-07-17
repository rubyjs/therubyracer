// -*- mode: c++ -*-
#ifndef RR_MAYBE_H
#define RR_MAYBE_H

namespace rr {

  /**
   * A base class for conversion objects that will return an instance
   * of `V8::C::Maybe` depending on whether a value is available.
   *
   * Sprinkled throughout the V8 API, there are methods that may or
   * may not return a value based on whether there is an Exception
   * that is pending in the context of that operation.
   *
   * This class provides the basis for helpers to convert the C++
   * Maybe<T> and MaybeLocal<T> objects into Ruby objects that force
   * you to check if an operation suceeded.
   *
   * By default, it will be `V8::C::Nothing`, but if, in the
   * constructor, you call `just(VALUE value)`, then it will take on
   * that value.
   */
  class Maybe {
  public:

    /**
     * V8::C::Just
     */
    static VALUE JustClass;

    /**
     * Singleton instance of `V8::C::Nothing`
     */
    static VALUE Nothing;

    /**
     * By default, everything starts out as nothing.
     */
    Maybe() : object(Nothing) {}

    /**
     * Convert this seemlessly to a VALUE, which in this case is just
     * the v8::C::Maybe ruby instance.
     */
    inline operator VALUE() {
      return object;
    }

    static inline void Init() {
      rb_eval_string("require 'v8/c/maybe'");
      JustClass = rb_eval_string("::V8::C::Maybe::Just");
      Nothing = rb_eval_string("::V8::C::Maybe.nothing");
    }

  protected:

    /**
     * Subclasses call this method in the constructor in order to
     * indicate that this is a `V8::C::Just`, and to pass the
     * underlying value.
     */
    inline void just(VALUE v) {
      object = rb_funcall(JustClass, rb_intern("new"), 1, v);
    }

    // the underlying value.
    VALUE object;
  };
}

#endif /* RR_MAYBE_H */
