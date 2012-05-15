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
    return (T)(RTEST(value) ? NUM2INT(value) : 0);
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
  Ref<T>(v8::Handle<T> handle) {
    this->holder = new Holder(handle, Class);
  }
  virtual operator VALUE() {
    return holder->value;
  }
  virtual operator v8::Handle<T>() {
    return holder->handle;
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
  static VALUE Class;
};
template <class T> VALUE Ref<T>::Class;

class Handles {
public:
  static void Init();
  static VALUE HandleScope(int argc, VALUE* argv, VALUE self);
private:
  static VALUE SetupAndCall(int* state, VALUE code);
  static VALUE DoCall(VALUE code);
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
  static VALUE Global(VALUE self);
  static VALUE DetachGlobal(VALUE self);
  static VALUE ReattachGlobal(VALUE self, VALUE global);
  static VALUE GetEntered(VALUE self);
  static VALUE GetCurrent(VALUE self);
  static VALUE GetCalling(VALUE self);
  static VALUE SetSecurityToken(VALUE self, VALUE token);
  static VALUE UseDefaultSecurityToken(VALUE self);
  static VALUE GetSecurityToken(VALUE self);
  static VALUE HasOutOfMemoryException(VALUE self);
  static VALUE InContext(VALUE self);
  static VALUE SetData(VALUE self, VALUE data);
  static VALUE GetData(VALUE self);
  static VALUE AllowCodeGenerationFromStrings(VALUE self, VALUE allow);
  static VALUE IsCodeGenerationFromStringsAllowed(VALUE self);

  inline Context(VALUE value) : Ref<v8::Context>(value) {}
  inline Context(v8::Handle<v8::Context> cxt) : Ref<v8::Context>(cxt) {}
};

class External: public Ref<v8::External> {
public:
  static void Init();
  static VALUE New(VALUE self, VALUE data);
  static VALUE Value(VALUE self);

  inline External(VALUE value) : Ref<v8::External>(value) {}
  inline External(v8::Handle<v8::External> ext) : Ref<v8::External>(ext) {}
  static v8::Handle<v8::External> wrap(VALUE data);
  static VALUE unwrap(v8::Handle<v8::External> external);
private:
  static void release(v8::Persistent<v8::Value> object, void* parameter);
  struct Data {
    Data(VALUE data);
    ~Data();
    VALUE value;
  };
};

class Script : public Ref<v8::Script> {
public:
  static void Init();
  static VALUE New(VALUE klass, VALUE source, VALUE filename);
  static VALUE Run(VALUE self);

  inline Script(VALUE value) : Ref<v8::Script>(value) {}
  inline Script(v8::Handle<v8::Script> script) : Ref<v8::Script>(script) {}
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
  static VALUE Concat(VALUE self, VALUE left, VALUE right);

  inline String(VALUE value) : Ref<v8::String>(value) {}
  inline String(v8::Handle<v8::String> string) : Ref<v8::String>(string) {}
};

class PropertyAttribute: public Enum<v8::PropertyAttribute> {
public:
  inline PropertyAttribute(VALUE value) : Enum<v8::PropertyAttribute>(value) {}
};
class AccessControl: public Enum<v8::AccessControl> {
public:
  inline AccessControl(VALUE value) : Enum<v8::AccessControl>(value) {}
};

class Accessor {
public:
  static void Init();
  Accessor(const v8::AccessorInfo& info);
  ~Accessor();
  static VALUE This(VALUE self);
  static VALUE Holder(VALUE self);
  static VALUE Data(VALUE self);
  operator VALUE();
  v8::Handle<v8::Value> get(v8::Local<v8::String> property);
  v8::Handle<v8::Value> set(v8::Local<v8::String> property, v8::Local<v8::Value> value);
  static v8::Handle<v8::Value> AccessorGetter(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void AccessorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  class Info {
  public:
    Info(VALUE getter, VALUE setter, VALUE data);
    Info(v8::Local<v8::Value> value);
    operator v8::Handle<v8::Value>();
    void mark();
    v8::AccessorGetter Getter();
    v8::AccessorSetter Setter();
    VALUE getter;
    VALUE setter;
    VALUE data;
  private:
    void wrap(v8::Handle<v8::Object> wrapper, uint32_t index, VALUE value);
    VALUE unwrap(v8::Handle<v8::Object> wrapper, uint32_t index);
  };
private:
  VALUE thisObject;
  VALUE holder;
  VALUE value;
  Info* info;
  static void mark(Accessor* accessor);
  static void sweep(Accessor* accessor);
  static Accessor* accessor(VALUE self);
  static VALUE Class;
};

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
  static VALUE SetAccessor(int argc, VALUE* argv, VALUE self);

  inline Object(VALUE value) : Ref<v8::Object>(value) {}
  inline Object(v8::Handle<v8::Object> object) : Ref<v8::Object>(object) {}
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
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(int, VALUE*, VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(int, VALUE*, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
  ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
  ClassBuilder& defineEnumConst(const char* name, int value);
  ClassBuilder& store(VALUE* storage);
  inline operator VALUE() {return this->value;}
private:
  VALUE value;
};

}

#endif
