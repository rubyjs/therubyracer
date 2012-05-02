#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <v8.h>
#include <ruby.h>

namespace rr {

class Value {
public:
  Value(v8::Handle<v8::Value> handle);
  Value(VALUE value);
  operator VALUE();
  operator v8::Handle<v8::Value>();
  static void Init();
protected:
  v8::Handle<v8::Value> value;
  VALUE object;
};

class String : public Value {
public:
  String(VALUE value);
  operator v8::Handle<v8::String>();
  static void Init();
};

/**
* A Reference to a V8 managed object
*/
template <class T> class Ref {
public:
  Ref<T>(VALUE wrapper) {
    Holder* holder = NULL;
    Data_Get_Struct(wrapper, class Holder, holder) ;
    this->holder = holder;
  }
  virtual operator VALUE() {
    return holder->value;
  }
  virtual operator v8::Handle<T>() {
    return holder->handle;
  }
  static Ref<T> create(v8::Handle<T> handle, VALUE klass) {
    return Ref<T>(new Holder(handle, klass));
  }
  inline v8::Handle<T> operator->() const { return holder->handle; }
private:
  class Holder {
    friend class Ref;
  public:
    Holder(v8::Handle<T> handle, VALUE klass) {
      this->handle = v8::Persistent<T>::New(handle);
      this->value = Data_Wrap_Struct(klass, 0, &Holder::enqueue, this);
    }
    virtual ~Holder() {
      handle.Dispose();
    }
  protected:
    VALUE value;
    v8::Persistent<T> handle;

    static void enqueue(Holder* holder) {
      holder->value = Qnil;
      printf("enqueuing a reference for garbage collection (should actually do something)\n");
    }
  };
  Ref(Holder* holder) {
    this->holder = holder;
  };
  Holder* holder;
};

class Context : public Ref<v8::Context> {
public:
  inline Context(VALUE value) : Ref<v8::Context>(value) {};
  static void Init();
};

class Script : public Ref<v8::Script> {
public:
  inline Script(VALUE value) : Ref<v8::Script>(value) {};
  static void Init();
};

VALUE defineClass(const char *name, VALUE superclass = rb_cObject);
VALUE defineModule(const char *name);
}

#define RR_DEFINE_METHOD(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define RR_DEFINE_SINGLETON_METHOD(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

#endif
