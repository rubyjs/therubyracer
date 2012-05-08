#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <v8.h>
#include <ruby.h>

namespace rr {

VALUE Convert(bool);
VALUE Convert(v8::Handle<v8::Value> handle);

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
* A V8 Enum
*/

template <class T> class Enum {
public:
  Enum<T>(VALUE value) {
    this->value = value;
  }
  inline operator T() {
    return NIL_P(value) ? 0 : NUM2INT(value);
  }
private:
  VALUE value;
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
  static void Init();
  static VALUE New(VALUE self);
  static VALUE Enter(VALUE self);
  static VALUE Exit(VALUE self);

private:
  inline Context(VALUE value) : Ref<v8::Context>(value) {}
};

class Script : public Ref<v8::Script> {
public:
  static void Init();
  static VALUE New(VALUE klass, VALUE source, VALUE filename);
  static VALUE Run(VALUE self);

private:
  inline Script(VALUE value) : Ref<v8::Script>(value) {}
};

class Value : public Ref<v8::Value> {
public:
  static void Init();
  static VALUE Equals(VALUE self, VALUE other);
  static VALUE StrictEquals(VALUE self, VALUE other);
  inline Value(VALUE value) : Ref<v8::Value>(value) {}
};

class String: public Ref<v8::String> {
public:
  static void Init();
  static VALUE New(VALUE self, VALUE value);
  static VALUE Utf8Value(VALUE self);

  static VALUE Convert(v8::Handle<v8::String> value);

  inline String(VALUE value) : Ref<v8::String>(value) {}
private:
  static VALUE Class;
};

class PropertyAttribute: public Enum<v8::PropertyAttribute> {};
class AccessControl: public Enum<v8::AccessControl> {};

class Object : public Ref<v8::Object> {
public:
  static void Init();
  static VALUE New(VALUE self);
  static VALUE Set(VALUE self, VALUE key, VALUE value);
  static VALUE ForceSet(VALUE self, VALUE key, VALUE value);
  static VALUE Get(VALUE self, VALUE key);
  static VALUE GetPropertyAttributes(VALUE self, VALUE key);
  static VALUE Has(VALUE self, VALUE key);
  static VALUE Delete(VALUE self, VALUE key);
  static VALUE ForceDelete(VALUE self, VALUE key);

  inline Object(VALUE value) : Ref<v8::Object>(value) {}
};

class V8 {
public:
  static void Init();
  static VALUE IdleNotification(VALUE self);
};

class ClassBuilder {
public:
  ClassBuilder(const char* name, VALUE superclass = rb_cObject);
  ClassBuilder(const char* name, const char* supername);
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
  ClassBuilder& defineEnumConst(const char* name, int value);
  inline operator VALUE() {return this->value;}
private:
  VALUE value;
};

}

#endif
