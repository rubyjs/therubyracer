// -*- mode: c++ -*-
#ifndef RR_WRAPPER_H
#define RR_WRAPPER_H

namespace rr {

  /**
   * A wrapper provides the ability to create a temporary reference to
   * a stack-allocated value so that it can be passed to ruby. This is
   * normally the case for a callback parameter, where you will be
   * calling into Ruby from C++. Ruby should not call methods on this
   * object outside the scope of the function call. E.g.
   *
   *   class MyObjectWrapper : public Wrapper<MyObject> {
   *     // [...]
   *   };
   *
   *  MyObject foo;
   *  rb_funcall(object, rb_intern("count"), 1, MyObjectWrapper(foo));
   *  // ruby should not use anymore, it might crash
   *
   * Note: This seems dangerous in practice, but since it is only ever
   * used by the low-level api, wrapper objects do not escape into the
   * wild, and so aren't used beyond their normal lifetime on the C++
   * stack.
   *
   * We may add a destructor that invalidates the pointer,
   * and would throw an exception if you tried to use this object at
   * some after it leaves scope.
   */
  template <class T>
  class Wrapper {
  public:
    /**
     * The Ruby Class of this Wrapper
     */
    static VALUE Class;

    /**
     * Package up this wrapper so that it can be accessed from
     * Ruby. Use this just before passing to a ruby call. E.g.
     *
     *   MyObject object;
     *   rb_funcall(thing, rb_intern("take"), 1, MyObjectWrapper(object))
     */
    Wrapper(T content) : container(new Container(content)) {}

    /**
     * Access the underlying object when it has been passed from Ruby
     * to a C++ function. E.g.
     *
     *   SomeFunction(VALUE self) {
     *     MyObjectWrapper object(self);
     *   }
     */
    Wrapper(VALUE self) : container(Container::unwrap(self)) {}

    /**
     * Access the wrapped value via pointer dereference:
     *
     *   MyObjectWrapper object(self);
     *   object->SomeMethod();
     */
    inline T* operator ->() {
      return &container->content;
    }

    /**
     * Convert this Wrapper into a Ruby VALUE so that it can be
     * transparently passed to methods expecting a VALUE. This means
     * you can do things like:
     *
     *   MyObjectWrapper wrapper(self);
     *   rb_inspect(wrapper);
     *
     * Even though MyObjectWrapper is not a VALUE
     */
    inline operator VALUE() {
      return Container::wrap(container, Class);
    }

    /**
     * This is the struct that will be held inside the Ruby T_DATA
     * object. It mainly serves to keep a reference to the stack value.
     */
    struct Container {

      Container(T content_) : content(content_) {}

      static inline VALUE wrap(Container* container, VALUE rb_class) {
        return Data_Wrap_Struct(rb_class, 0, &destroy, container);
      }

      static inline Container* unwrap(VALUE object) {
        Container* container;
        Data_Get_Struct(object, struct Container, container);
        return container;
      }

      static void destroy(Container* container) {
        delete container;
      }
      T content;
    };

    Container* container;
  };

  template <class T>
  VALUE Wrapper<T>::Class;
}

#endif /* RR_WRAPPER_H */
