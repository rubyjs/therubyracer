#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <v8.h>
#include <ruby.h>

namespace rr {
  /**
  * A Reference to a V8 managed object
  */
  template <class T> class Ref {
  public:
    Ref<T>(VALUE wrapper);
    virtual operator VALUE();
    static Ref<T> create(v8::Handle<T> handle, VALUE klass);
    inline v8::Handle<T> operator->() const { return holder->handle; }
  private:
    class Holder {
      Holder(v8::Handle<T> handle, VALUE klass);
      virtual ~Holder();

      VALUE value;
      v8::Persistent<T> handle;
      friend class Ref<T>;

      static void enqueue(Holder* holder);
    };
    Ref(Holder* holder);
    Holder* holder;
  };

  class Context : public Ref<v8::Context> {
  public:
    inline Context(VALUE value) : Ref<v8::Context>(value) {};
    static void Init();
  };

  VALUE defineClass(const char *name, VALUE superclass = rb_cObject);
  VALUE defineModule(const char *name);
}

#define RR_DEFINE_METHOD(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define RR_DEFINE_SINGLETON_METHOD(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

#endif
