// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_H
#define RR_PROPERTY_CALLBACK_H

namespace rr {

  class PropertyCallback {
  public:

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
    static v8::Local<v8::Value> wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter, VALUE r_data);
    static v8::Local<v8::Value> wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter,
                                         VALUE r_query, VALUE r_deleter, VALUE r_enumerator, VALUE r_data);

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

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::AccessorNameSetterCallback` API.
     */
    static void invokeSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyGetterCallback` API.
     */
    static void invokeNamedPropertyGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertySetterCallback` API.
     */
    static void invokeNamedPropertySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyQueryCallback` API.
     */
    static void invokeNamedPropertyQuery(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Integer>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyDeleterCallback` API.
     */
    static void invokeNamedPropertyDeleter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Boolean>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyEnumeratorCallback` API.
     */
    static void invokeNamedPropertyEnumerator(const v8::PropertyCallbackInfo<v8::Array>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyGetterCallback` API.
     */
    static void invokeIndexedPropertyGetter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertySetterCallback` API.
     */
    static void invokeIndexedPropertySetter(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyQueryCallback` API.
     */
    static void invokeIndexedPropertyQuery(uint32_t index, const v8::PropertyCallbackInfo<v8::Integer>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::NamedPropertyDeleterCallback` API.
     */
    static void invokeIndexedPropertyDeleter(uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info);

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::IndexedPropertyEnumeratorCallback` API.
     */
    static void invokeIndexedPropertyEnumerator(const v8::PropertyCallbackInfo<v8::Array>& info);
  };

}

#endif /* RR_PROPERTY_CALLBACK_H */
