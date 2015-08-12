// -*- mode: c++ -*-
#ifndef RR_EXCEPTION_H
#define RR_EXCEPTION_H
#include "rr.h"

namespace rr {
  class Exception {
  public:
    static inline void Init() {
      ClassBuilder("Exception").
        defineSingletonMethod("RangeError", &RangeError).
        defineSingletonMethod("ReferenceError", &ReferenceError).
        defineSingletonMethod("SyntaxError", &SyntaxError).
        defineSingletonMethod("TypeError", &TypeError).
        defineSingletonMethod("Error", &Error);
    }

    static VALUE RangeError(VALUE self, VALUE rb_message) {
      String message(rb_message);
      Locker lock(message);
      return Value(message, v8::Exception::RangeError(message));
    }

    static VALUE ReferenceError(VALUE self, VALUE rb_message) {
      String message(rb_message);
      Locker lock(message);
      return Value(message, v8::Exception::ReferenceError(message));
    }

    static VALUE SyntaxError(VALUE self, VALUE rb_message) {
      String message(rb_message);
      Locker lock(message);
      return Value(message, v8::Exception::SyntaxError(message));
    }

    static VALUE TypeError(VALUE self, VALUE rb_message) {
      String message(rb_message);
      Locker lock(message);
      return Value(message, v8::Exception::TypeError(message));
    }

    static VALUE Error(VALUE self, VALUE rb_message) {
      String message(rb_message);
      Locker lock(message);
      return Value(message, v8::Exception::Error(message));
    }
  };
}

#endif /* RR_EXCEPTION_H */
