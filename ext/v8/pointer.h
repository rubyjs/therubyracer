#ifndef RR_POINTER
#define RR_POINTER

namespace rr {

  /**
  * A pointer to V8 object managed by Ruby
  *
  * You deal with V8 objects as either pointers or handles.
  * While handles are managed by the V8 garbage collector, pointers
  * must be explicitly created and destroyed by your code.
  *
  * The pointer class provides a handly way to wrap V8 pointers
  * into Ruby objects so that they will be deleted when the
  * Ruby object is garbage collected. Automatic type coercion is
  * used to make wrapping and unwrapping painless.
  *
  * To create Ruby VALUE:
  *
  *  Pointer<v8::ScriptOrigin> ptr(new v8::ScriptOrigin());
  *  VALUE value = ptr; //automatically wraps in Data_Wrap_Struct
  *
  * Conversely, the pointer can be unwrapped from a struct
  * created in this way and the underlying methods can be
  * invoked:
  *
  *     VALUE value = ...;
  *     Pointer<v8::ScriptOrigin> ptr(value);
  *     ptr->CallMethod();
  */
  template <class T>
  class Pointer {
  public:
    inline Pointer(T* t) : pointer(t) {};
    inline Pointer(VALUE v) {
      if (RTEST(v)) {
        this->unwrap(v);
      } else {
        this->pointer = NULL;
      }
    };

    inline operator T*() {
      return pointer;
    }

    inline T* operator ->() {
      return pointer;
    }

    inline operator VALUE() {
      return Data_Wrap_Struct(Class, 0, &release, pointer);
    }

    void unwrap(VALUE value);

    static void release(T* pointer) {
      delete pointer;
    }

    static VALUE Class;

    static inline VALUE PointerEquals(VALUE self, VALUE other) {
      return Bool(Pointer<T>(self).pointer == Pointer<T>(other).pointer);
    }

  protected:
    T* pointer;
  };

  template <class T> VALUE Pointer<T>::Class;

}

#endif
