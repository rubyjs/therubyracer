#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <v8.h>
#include <ruby.h>

namespace rr {

class Value {
public:
  Value(v8::Handle<v8::Value> handle);
  Value(VALUE value);
  virtual operator VALUE();
  virtual operator v8::Handle<v8::Value>();
  static void Init();
protected:
  v8::Handle<v8::Value> value;
  VALUE object;
};

class GC {
public:
  class Queue {
  public:
    Queue();
    void Enqueue(void* phantom);
    void* Dequeue();
    private:
      struct Node {
        Node(void* val ) : value(val), next(NULL) { }
        void* value;
        Node* next;
      };
      Node* first;      // for producer only
      Node* divider;
      Node* last;
  };
  static void Finalize(void* phantom);
  static void Drain(v8::GCType type, v8::GCCallbackFlags flags);
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
  v8::Handle<T> GetHandle() {return holder->handle;}

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
      GC::Finalize(holder);
    }
  };
  Ref(Holder* holder) {
    this->holder = holder;
  };
  Holder* holder;
};

class Phantom : public Ref<void> {
public:
  inline Phantom(void* reference) : Ref<void>((Ref<void>::Holder*)reference) {}
  inline bool NotNull() {
    return this->holder != NULL;
  }
  inline void destroy() {
    delete holder;
  }
};

class Context : public Ref<v8::Context> {
public:
  inline Context(VALUE value) : Ref<v8::Context>(value) {}
  static void Init();
};

class Script : public Ref<v8::Script> {
public:
  inline Script(VALUE value) : Ref<v8::Script>(value) {}
  static void Init();
};

class String: public Ref<v8::String> {
public:
  inline String(VALUE value) : Ref<v8::String>(value) {}
  virtual operator v8::Handle<v8::Value>();
  static VALUE ToRuby(v8::Handle<v8::String> value);
  static void Init();
  static VALUE Class;
};

class V8 {
public:
  static void Init();
};

class ClassBuilder {
public:
  ClassBuilder(const char* name, VALUE superclass = rb_cObject);
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  inline operator VALUE() {return this->value;}
private:
  VALUE value;
};

VALUE defineClass(const char *name, VALUE superclass = rb_cObject);
VALUE defineModule(const char *name);
}

#define RR_DEFINE_METHOD(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)
#define RR_DEFINE_SINGLETON_METHOD(object, name, impl, argc) rb_define_singleton_method(object, name, (VALUE(*)(...))impl, argc)

#endif
