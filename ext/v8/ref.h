#ifndef RR_REF
#define RR_REF

namespace rr {
  /**
  * A Reference to a V8 managed object
  *
  * Uses type coercion to quickly convert from a v8 handle
  * to a ruby object and back again. Suppose we have a v8 handle
  * that we want to return to Ruby. We can put it into a Ref:
  *
  *     v8::Handle<v8::Object> object = v8::Object::New();
  *     VALUE val = Ref<v8::Object>(object);
  *
  * this will create a `v8::Persistent` handle for the object
  * so that it will not be garbage collected by v8. It then
  * stuffs this new persistent handle into a Data_Wrap_Struct
  * which can then be passed to Ruby code. When this struct
  * is garbage collected by Ruby, it enqueues the corresponding
  * v8 handle to be released during v8 gc.
  *
  * By the same token, you can use Refs to unwrap a Data_Wrap_Struct
  * which has been generated in this fashion and call through to
  * the underlying v8 methods. Suppose we are passed a VALUE `val`
  * wrapping a v8::Object:
  *
  *     Ref<v8::Object> object(val);
  *     object->Get(v8::String::New("foo"));
  *
  */
  template <class T>
  class Ref {
  public:
    Ref(VALUE value) {
      this->value = value;
    }

    Ref(v8::Local<T> handle) {
      this->handle = handle;
    }

    virtual ~Ref() {}

    /*
    *  Coerce a Ref into a Ruby VALUE
    */
    virtual operator VALUE() const {
      if (handle.IsEmpty()) {
        return Qnil;
      }

      return Data_Wrap_Struct(Class, 0, &Holder::destroy, new Holder(handle));
    }

    /*
    * Coerce a Ref into a v8::Local.
    */
    virtual operator v8::Handle<T>() const {
      if (RTEST(this->value)) {
        Holder* holder = NULL;
        Data_Get_Struct(this->value, class Holder, holder);

        return v8::Local<T>::New(v8::Isolate::GetCurrent(), *holder->handle);
      } else {
        return v8::Local<T>();
      }
    }

    void dispose() {
      Holder* holder = NULL;
      Data_Get_Struct(this->value, class Holder, holder);
      holder->dispose();
    }

    /*
    * Pointer de-reference operators, this lets you use a ref to
    * call through to underlying v8 methods. e.g
    *
    *     Ref<v8::Object>(value)->ToString();
    */
    inline v8::Handle<T> operator->() const { return *this; }
    inline v8::Handle<T> operator*() const { return *this; }

    class Holder {
      friend class Ref;
    public:
      Holder(v8::Handle<T> handle) {
        this->disposed_p = false;
        this->handle = new v8::Persistent<T>(v8::Isolate::GetCurrent(), handle);
      }

      virtual ~Holder() {
        this->dispose();
      }

      void dispose() {
        if (!this->disposed_p) {
          handle->Reset();
          delete handle;
          this->disposed_p = true;
        }
      }

    protected:
      v8::Persistent<T>* handle;
      bool disposed_p;

      static void destroy(Holder* holder) {
        holder->dispose();

        // TODO: This previously enqueued the holder to be disposed of
        // in `AddGCPrologueCallback`. Now that `AddGCPrologueCallback` depends
        // on an active Isolate (and must be registered for each one) it
        // might be better to just dispose of the object on the spot.
        // GC::Finalize(holder);
      }
    };

    VALUE value;
    v8::Handle<T> handle;
    static VALUE Class;
  };

  template <class T>
  VALUE Ref<T>::Class;

}

#endif
