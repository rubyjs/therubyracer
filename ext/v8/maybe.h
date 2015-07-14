// -*- mode: c++ -*-
#ifndef RR_MAYBE_H
#define RR_MAYBE_H


/**
 * Helpers to deal with V8 apis that may or may not return values
 * because they have an exception pending. They come in too flavors:
 * One for simple C++ values, and other for persistent
 * references. These correspond to the v8::Maybe, and v8::MaybeLocal
 * classes.
 *
 * When you unbox a return value with a `Maybe` or a `MaybeLocal`, it
 * will check to see if there is a value. If so, it returns the
 * value. If not, it raises an instance of `V8::C::PendingExceptionError`
 *
 * To use, provide the template with two classes: 1) the v8 internal
 * class that underlying method returns, and 2) the converter class that
 * will convert it into a Ruby `VALUE`. For example, to wrap a call to
 * `v8::Maybe<bool> Has(v8::Local<v8::Context> context)` in the class
 * `v8::Object`, you would want to say something like:
 *
 *   return Maybe<bool,Bool>(object->Has(context, key));
 *
 * In practice, in the code, you'll usually see a typedef so that you
 * don't have to constantly type out the same template parameters over
 * and over again. So in the example above Bool::Maybe is typedef'd to
 * Maybe<bool, Bool>, so you could also have written:
 *
 *   return Bool::Maybe(object->Has(context, key));
 */
namespace rr {
  /**
   * Holds the Ruby exception class for `V8::C::PendingExceptionError`
   */
  static VALUE PendingExceptionError;

  template <class T, class E>
  class Maybe {
  public:
    Maybe(v8::Maybe<T>& maybe_) : maybe(maybe_) {}

    inline operator VALUE() {
      if (maybe.IsNothing()) {
        rb_raise(PendingExceptionError, "operation cannot be performed due to a pending exception in this context\n");
        return Qnil;
      } else {
        return E(maybe.FromJust());
      }
    }

    v8::Maybe<T>  maybe;
  };

  template <class T, class R>
  class MaybeLocal {
  public:
    MaybeLocal(v8::Isolate* isolate_, v8::MaybeLocal<T> maybe_) :
      isolate(isolate_), maybe(maybe_) {}

    inline operator VALUE() {
      if (maybe.IsEmpty()) {
        rb_raise(PendingExceptionError, "operation cannot be performed due to a pending exception in this context\n");
        return Qnil;
      } else {
        return R(isolate, maybe.ToLocalChecked());
      }
    }
    v8::Isolate* isolate;
    v8::MaybeLocal<T> maybe;
  };

  class MaybeInit {
  public:
    static void Init() {
      ClassBuilder("PendingExceptionError", rb_eStandardError).
        store(&PendingExceptionError);
    }
  };
};

#endif /* RR_MAYBE_H */
