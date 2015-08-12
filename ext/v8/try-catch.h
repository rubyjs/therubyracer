// -*- mode: c++ -*-

#ifndef RR_TRY_CATCH_H
#define RR_TRY_CATCH_H
#include "rr.h"

namespace rr {
  /**
   * V8::C::TryCatch is both a class and a method.
   *
   * Use the method to evaluate a block of Ruby code from within a
   * v8::TryCatch. The block will receive an instance of the
   * `V8::C::TryCatch` class as its argument E.g.
   *
   *   V8::C::TryCatch(isolate) do |trycatch|
   *     trycatch.class #=> V8::C::TryCatch
   *     cxt.eval('throw new Error()')
   *     trycatch.HasCaught() #=> true
   *   end
   *
   * Note that the `trycatch` value yielded to the block must never be
   * used outside the block since it references the `v8::TryCatch` C++
   * object which is allocated on the stack. See `doTryCatch` and
   * `call` for the implementation of the block parameters.
   *
   * Note: Ideally, we'd like to make the `Wrapper` class as a super
   * class to handle the wrapping and unwrapping of the TryCatch, but
   * that would require introducing pass-by-reference semantics to the
   * wrapper template (since v8::TryCatch disables the copy
   * constructor). Satisfying the compiler proved too challenging for
   * this poor implementer.
   */
  class TryCatch {
  public:
    TryCatch(VALUE self) : container(Container::unwrap(self)) {}
    TryCatch(v8::Isolate* isolate, v8::TryCatch& trycatch) :
      container(new Container(isolate, trycatch)) {}

    static VALUE HasCaught(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return Bool(tc->HasCaught());
    }
    static VALUE CanContinue(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return Bool(tc->CanContinue());
    }
    static VALUE HasTerminated(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return Bool(tc->HasTerminated());
    }
    static VALUE ReThrow(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return Value(tc, tc->ReThrow());
    }
    static VALUE Exception(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return Value(tc, tc->Exception());
    }
    static VALUE StackTrace(VALUE self, VALUE context) {
      TryCatch tc(self);
      Locker lock(tc);
      return Value::Maybe(tc, tc->StackTrace(Context(context)));
    }
    static VALUE Message(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      return rr::Message(tc, tc->Message());
    }
    static VALUE Reset(VALUE self) {
      TryCatch tc(self);
      Locker lock(tc);
      tc->Reset();
      return Qnil;
    }
    static VALUE SetVerbose(VALUE self, VALUE value) {
      TryCatch tc(self);
      Locker lock(tc);
      tc->SetVerbose(Bool(value));
      return Qnil;
    }
    static VALUE SetCaptureMessage(VALUE self, VALUE value) {
      TryCatch tc(self);
      Locker lock(tc);
      tc->SetCaptureMessage(Bool(value));
      return Qnil;
    }

    /**
     * Implements the V8::C::TryCatch(isolate) method. This yields the
     * trycatch to the passed ruby code.
     */
    static VALUE doTryCatch(int argc, VALUE argv[], VALUE self) {
      if (!rb_block_given_p()) {
        return Qnil;
      }
      int state;
      VALUE isolate, code, result;
      rb_scan_args(argc, argv, "10&", &isolate, &code);

      // allocate the trycatch within its own scope, so that its
      // destructor is called, even if we're forced to `rb_jmp_tag`
      // out of this function because of a ruby exception.
      // the actual `V8::C::TryCatch` object is stored on the code in
      // the `_v8_trycatch` variable, because rb_protect only accepts
      // a single argument.
      {
        v8::TryCatch trycatch;
        rb_iv_set(code, "_v8_trycatch", TryCatch(Isolate(isolate), trycatch));
        result = rb_protect(&call, code, &state);
        rb_iv_set(code, "_v8_trycatch", Qnil);
      }
      if (state != 0) {
        // ruby exception. bail!
        rb_jump_tag(state);
      }
      return result;
    }
    /**
     * Actually invokes the passed ruby block. The instance of
     * `V8::C::TryCatch` is pulled off of the `_v8_trycatch` instance variable.
     */
    static VALUE call(VALUE code) {
      return rb_funcall(code, rb_intern("call"), 1, rb_iv_get(code, "_v8_trycatch"));
    }


    // What follows is a copy of the `Wrapper` template, that uses
    // pass-by-reference semantics so that the actual `v8::TryCatch`
    // object that is allocated on the C++ stack is the same object
    // used everywhere. Ideally, we'd like to share the
    // implementation, but I couldn't figure out how.
    static VALUE Class;

    struct Container {
      Container(v8::Isolate* isolate_, v8::TryCatch& tc) : isolate(isolate_), trycatch(tc) {}

      inline VALUE wrap() {
        return Data_Wrap_Struct(Class, 0, &destroy, this);
      }

      static inline Container* unwrap(VALUE object) {
        Container* container;
        Data_Get_Struct(object, struct Container, container);
        return container;
      }

      static void destroy(Container* container) {
        delete container;
      }
      v8::Isolate* isolate;
      v8::TryCatch& trycatch;
    };

    inline v8::TryCatch* operator ->() {
      return &container->trycatch;
    }

    inline operator v8::Isolate*() {
      return container->isolate;
    }

    inline operator VALUE() {
      return container->wrap();
    }
    static inline void Init() {
      ClassBuilder("TryCatch").
        defineMethod("HasCaught", &HasCaught).
        defineMethod("CanContinue", &CanContinue).
        defineMethod("HasTerminated", &HasTerminated).
        defineMethod("ReThrow", &ReThrow).
        defineMethod("Exception", &Exception).
        defineMethod("StackTrace", &StackTrace).
        defineMethod("Message", &Message).
        defineMethod("Reset", &Reset).
        defineMethod("SetVerbose", &SetVerbose).
        defineMethod("SetCaptureMessage", &SetCaptureMessage).

        store(&Class);
      rb_define_singleton_method(rb_eval_string("V8::C"), "TryCatch", (VALUE (*)(...))&doTryCatch, -1);
    }

    Container* container;
  };
}


#endif /* RR_TRY_CATCH_H */
