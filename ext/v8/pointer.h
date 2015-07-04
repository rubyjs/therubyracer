// -*- mode: c++ -*-
#ifndef RR_POINTER
#define RR_POINTER

namespace rr {

  /**
   * Conversion between a native C/C++ pointer and a Ruby Object.
   *
   * Use this class When you have a native C/C++ pointer that you want
   * to make appear as a Ruby object. It will take care of all the
   * book keeping like registering GC callbacks. It is also used to
   * access from C/C++ a pointer that you had previously passed to Ruby.
   *
   * Suppose we have a C++ class `Greeter`.
   *
   *   class Greeter {
   *     public:
   *     void sayHello() {
   *       println("Hello");
   *     }
   *   }
   *
   * and that we want to pass an instance of that class to the
   * following Ruby method:
   *
   *   class Greet
   *     def self.greet(greeter)
   *       //we want greeter to wrap our C++ object.
   *       greeter.say_hello()
   *     end
   *   end
   *
   * We could do that from C by using a `Pointer` specified for
   * `Greeter`:
   *
   *   Greeter* greeter = new Greeter();
   *
   *   // get reference to Ruby `Greet` class from C
   *   VALUE GreeterClass = rb_define_class("Greet");
   *   Pointer<Greeter> native_greeter(greeter);
   *
   *   //now we can pass it to Ruby's `Greeter#new`
   *   rb_funcall(GreeterClass, rb_intern("greet"), 1, native_greeter);
   *
   * Once it is passed to Ruby, it is NO LONGER YOURS. It's lifecycle
   * now belongs to Ruby, and you must not delete the pointer by
   * hand.
   *
   * If we were to run the example above it would actually fail
   * because by default, the Ruby Class of the native pointer is
   * actually an opaque `Object`, and there is no method `say_hello`
   * on `Object`. In order to make it work, we have to specify which
   * class we want Ruby to treat our pointer as. We do that by setting
   * the static Pointer<T>::Class variable.
   *
   *   Pointer<Greeter>::Class = rb_define_class("NativeGreeter");
   *
   * Now, when we pass `Pointer<Greeter>(new Greeter())` to Ruby code,
   * it will appear to have the class `NativeGreeter`. We can now
   * define our `say_hello` method on that class in order to call the
   * underlying C++ method. We can construct a `Pointer<Greeter>` from
   * an instance of `NativeGreeter` and use it to automatically unwrap
   * the C++ `Greeter` object and call its methods:
   *
   *
   *   static VALUE say_hello(VALUE self) {
   *     //unwrap the native `Greeter` and call its `sayHello()`
   *     Pointer<Greeter>(self)->sayHello();
   *     return Qnil;
   *   }
   *   VALUE NativeGreeter = Pointer<Greeter>::Class;
   *   rb_define_method(NativeGreeter, "say_hello", (VALUE (*)(...))&say_hello, 0);
   *
   * Notice how we were able to just call the `sayHello()` method
   * directly on the instance of Pointer<Greeter>? That's because it
   * will automatically unbox and dereference the pointer so that you
   * can automatically forward calls to the underlying object it wraps.
   *
   * Most of the time The Ruby Racer codebase uses subclassing to
   * specify a Pointer. For example, `rr:Isolate ` looks like this:
   *
   *   class Isolate : public Poiter<v8::Isolate> {
   *     //....
   *   }
   *
   * which means that it can be used in all the cases above without
   * any template specifier:
   *
   *   //wrap
   *   Isolate isolate(new v8::Isolate());
   *
   *   //unwrap
   *   VALUE rubyIsolate = getRubyObjectContainingIsolate();
   *   Isolate isolate(rubyIsolate);
   *
   *   //Ruby class for Isolate
   *   Isolate::Class //=> V8::C::Isolate
   *
   */
  template <class T> class Pointer {
  public:
    /**
     * Construct a `Pointer` from a C/C++ pointer. This is normally
     * done in order to convert it into a Ruby `VALUE`. E.g.
     *
     *   Greeter* greeter = new Greeter();
     *   rb_funcall(Pointer<Greeter>(greeter), rb_intern("to_s"));
     */
    inline Pointer(T* t) : pointer(t) {};

    /**
     * Construct a Pointer from a C/C++ pointer that was previously
     * passed to Ruby. Use this access and call methods on the
     * underlying pointer:
     *
     *   Pointer<Greeter>(rubyValue)->sayHello();
     */
    inline Pointer(VALUE v) {
      if (RTEST(v)) {
        this->unwrap(v);
      } else {
        this->pointer = NULL;
      }
    };

    /**
     * Enable transparent pointer dereferencing via the `*` operator:
     *
     *   *Pointer<Greeter>(new Greeter()).sayHello();
     */
    inline operator T*() {
      return pointer;
    }

    /**
     * Enable transparent pointer dereferencing via the `->` operator:
     *
     *   Pointer<Greeter>(new Greeter())->sayHello();
     */
    inline T* operator ->() {
      return pointer;
    }

    /**
     * Coerce this Pointer into a VALUE. Once this happens, either by
     * assigning it to variable of type VALUE, casting it to (VALUE),
     * or passing it to a function that takes a VALUE as a parameter,
     * this pointer now belongs to Ruby, and you should not `delete`
     * it.
     */
    inline operator VALUE() {
      VALUE RubyClass = Class ? Class : rb_cObject;
      return Data_Wrap_Struct(RubyClass, 0, &release, pointer);
    }

    /**
     * Subclasses implement this to uwrap their particular
     * datastructure. For example, the implementation for `Isolate`
     * looks like:
     *
     *   void Pointer<v8::Isolate>::unwrap(VALUE value) {
     *     Data_Get_Struct(value, class v8::Isolate, pointer);
     *   }
     *
     * TODO: I would really like to get rid of this and it
     * seems like it ought to be templatable, but I couldn't figure
     * out a way to get the C++ compiler to play ball. Basically, it
     * seems like there ought to be a generic function like:
     *
     *   void inline unwrap(VALUE value) {
     *     Dat_Get_Struct(value, class T, pointer);
     *   }
     *
     * but it wouldn't work for me.
     */
    void unwrap(VALUE value);


    /**
     * A static method to delete this pointer. It is implemented as a
     * static method so that a function pointer to it can be passed to
     * Ruby so that it can be called from Ruby when the object is
     * garbage collected.
     * TODO: rename to `destroy()`
     */
    static void release(T* pointer) {
      delete pointer;
    }

    /**
     * Storage for the Class variable. This is where you write to
     * determine what Ruby class this Pointer will have:
     *
     *   Pointer<Greeter>::Class = rb_define_class("NativeGreeter");
     */
    static VALUE Class;

    /**
     * See if two `Pointer<T>`s are equal by comparing their memory
     * addresses.
     *
     * TODO: overload the `==` operator
     */
    static inline VALUE PointerEquals(VALUE self, VALUE other) {
      return Bool(Pointer<T>(self).pointer == Pointer<T>(other).pointer);
    }

  protected:
    T* pointer;
  };

  /**
   * Some C++ template nonsense.
   */
  template <class T> VALUE Pointer<T>::Class;

}

#endif
