// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_H
#define RR_PROPERTY_CALLBACK_H

namespace rr {

  class PropertyCallback {
    v8::Isolate* isolate;
    VALUE r_getter, r_setter, r_query, r_deleter, r_enumerator, r_data;

  public:
    PropertyCallback(v8::Isolate* isolate_, VALUE r_getter_, VALUE r_setter_, VALUE r_data_)
      : isolate(isolate_), r_getter(r_getter_), r_setter(r_setter_), r_query(Qnil), r_deleter(Qnil),
        r_enumerator(Qnil), r_data(r_data_) {
    }

    PropertyCallback(v8::Isolate* isolate_, VALUE r_getter_, VALUE r_setter_,
                     VALUE r_query_, VALUE r_deleter_, VALUE r_enumerator_, VALUE r_data_)
      : isolate(isolate_), r_getter(r_getter_), r_setter(r_setter_), r_query(r_query_), r_deleter(r_deleter_),
        r_enumerator(r_enumerator_), r_data(r_data_) {
    }

    /**
     * Package up the callback data for this object so that it can
     * invoke Ruby callables.
     *
     * Each accessor can have one `v8::Value` associated with it
     * that is passed to the `SetAccessor` method. To support this
     * same API from ruby, we take the passed data constructor *and*
     * the callbacks and store them *both* in a single `v8::Object`
     * which we use for the C++ level callback data.
     */
    operator v8::Local<v8::Value>();

    inline operator v8::MaybeLocal<v8::Value>() {
      return v8::MaybeLocal<v8::Value>((v8::Local<v8::Value>)*this);
    }

    /**
     * Get the data packaged in the object.
     */
    static VALUE unwrapData(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Get the getter packaged in the object.
     */
    static VALUE unwrapGetter(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Get the setter packaged in the object.
     */
    static VALUE unwrapSetter(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Get the query callback packaged in the object.
     */
    static VALUE unwrapQuery(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Get the deleter packaged in the object.
     */
    static VALUE unwrapDeleter(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Get the enumerator packaged in the object.
     */
    static VALUE unwrapEnumerator(v8::Isolate* isolate, v8::Local<v8::Value> holder);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::AccessorNameGetterCallback` API.
     */
    static void invokeGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    inline operator v8::AccessorNameGetterCallback() {
      if (RTEST(r_getter)) {
        return &PropertyCallback::invokeGetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::AccessorNameSetterCallback` API.
     */
    static void invokeSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    inline operator v8::AccessorNameSetterCallback() {
      if (RTEST(r_setter)) {
        return &PropertyCallback::invokeSetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyGetterCallback` API.
     */
    static void invokeNamedPropertyGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    inline operator v8::NamedPropertyGetterCallback() {
      if (RTEST(r_getter)) {
        return &PropertyCallback::invokeNamedPropertyGetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertySetterCallback` API.
     */
    static void invokeNamedPropertySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    inline operator v8::NamedPropertySetterCallback() {
      if (RTEST(r_setter)) {
        return &PropertyCallback::invokeNamedPropertySetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyQueryCallback` API.
     */
    static void invokeNamedPropertyQuery(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Integer>& info);

    inline operator v8::NamedPropertyQueryCallback() {
      if (RTEST(r_query)) {
        return &PropertyCallback::invokeNamedPropertyQuery;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyDeleterCallback` API.
     */
    static void invokeNamedPropertyDeleter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Boolean>& info);

    inline operator v8::NamedPropertyDeleterCallback() {
      if (RTEST(r_deleter)) {
        return &PropertyCallback::invokeNamedPropertyDeleter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyEnumeratorCallback`
     * and `v8::IndexedPropertyEnumeratorCallback` API.
     */
    static void invokePropertyEnumerator(const v8::PropertyCallbackInfo<v8::Array>& info);

    /**
     * This handles both `v8::NamedPropertyEnumeratorCallback` and `v8::IndexedPropertyEnumeratorCallback`
     * since they are the same type.
     */
    inline operator v8::NamedPropertyEnumeratorCallback() {
      if (RTEST(r_enumerator)) {
        return &PropertyCallback::invokePropertyEnumerator;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyGetterCallback` API.
     */
    static void invokeIndexedPropertyGetter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);

    inline operator v8::IndexedPropertyGetterCallback() {
      if (RTEST(r_getter)) {
        return &PropertyCallback::invokeIndexedPropertyGetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertySetterCallback` API.
     */
    static void invokeIndexedPropertySetter(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    inline operator v8::IndexedPropertySetterCallback() {
      if (RTEST(r_setter)) {
        return &PropertyCallback::invokeIndexedPropertySetter;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyQueryCallback` API.
     */
    static void invokeIndexedPropertyQuery(uint32_t index, const v8::PropertyCallbackInfo<v8::Integer>& info);

    inline operator v8::IndexedPropertyQueryCallback() {
      if (RTEST(r_query)) {
        return &PropertyCallback::invokeIndexedPropertyQuery;
      } else {
        return 0;
      }
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyDeleterCallback` API.
     */
    static void invokeIndexedPropertyDeleter(uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info);

    inline operator v8::IndexedPropertyDeleterCallback() {
      if (RTEST(r_deleter)) {
        return &PropertyCallback::invokeIndexedPropertyDeleter;
      } else {
        return 0;
      }
    }
  };

}

#endif /* RR_PROPERTY_CALLBACK_H */
