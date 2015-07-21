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

  };

}

#endif /* RR_PROPERTY_CALLBACK_H */
