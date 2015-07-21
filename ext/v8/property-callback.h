// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_H
#define RR_PROPERTY_CALLBACK_H

namespace rr {

  class PropertyCallbackInfo {
  public:

    template <class T>
    class Base : public Wrapper<v8::PropertyCallbackInfo<T>> {
    public:

      inline Base(v8::PropertyCallbackInfo<T> info) :
        Wrapper<v8::PropertyCallbackInfo<T>>(info) {}

      inline Base(VALUE self) : Wrapper<v8::PropertyCallbackInfo<T>>(self) {}

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
      static v8::Local<v8::Value> wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter, VALUE r_data) {
        v8::Local<v8::Object> holder = v8::Object::New(isolate);

        v8::Local<v8::String> getter_key = v8::String::NewFromUtf8(isolate, "rr::getter");
        v8::Local<v8::String> setter_key = v8::String::NewFromUtf8(isolate, "rr::setter");
        v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");

        holder->SetHiddenValue(getter_key, External::wrap(isolate, r_getter));
        holder->SetHiddenValue(setter_key, External::wrap(isolate, r_setter));
        holder->SetHiddenValue(data_key, rr::Value(r_data));

        return holder;
      }

      static VALUE This(VALUE self) {
        Base<T> info(self);
        Locker lock(info->GetIsolate());
        return Object(info->GetIsolate(), info->This());
      }

      static VALUE Data(VALUE self) {
        Base<T> info(self);
        Isolate isolate(info->GetIsolate());
        Locker lock(isolate);

        v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info->Data());
        v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
        v8::Local<v8::Value> data(holder->GetHiddenValue(data_key));

        return rr::Value::handleToRubyObject(info->GetIsolate(), data);
      }

      static VALUE GetIsolate(VALUE self) {
        Base<T> info(self);
        return Isolate(info->GetIsolate());
      }

    };

    class Value : public Base<v8::Value> {
    public:

      inline Value(v8::PropertyCallbackInfo<v8::Value> info) :
        Base<v8::Value>(info) {}

      inline Value(VALUE self) : Base<v8::Value>(self) {}

      /**
       * Call the Ruby code associated with this callback.
       *
       * Unpack the Ruby code, and the callback data from the C++
       * callback data, and then invoke that code.
       *
       * Note: This function implements the `v8::AccessorNameGetterCallback` API.
       */
      static void invoke(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info.Data());
        v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::getter");

        VALUE code(External::unwrap(holder->GetHiddenValue(callback_key)));

        VALUE rb_property;
        if (property->IsSymbol()) {
          rb_property = Symbol(isolate, v8::Local<v8::Symbol>::Cast(property));
        } else {
          rb_property = String(isolate, property->ToString());
        }

        Unlocker unlock(info.GetIsolate());
        rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)Value(info));
      }

      static VALUE GetReturnValue(VALUE self) {
        Value info(self);
        Locker lock(info->GetIsolate());
        return ReturnValue(info->GetReturnValue());
      }

      static inline void Init() {
        ClassBuilder("Value", PropertyCallbackInfo::Class, PropertyCallbackInfo::Class).
          defineMethod("This", &This).
          defineMethod("Data", &Data).
          defineMethod("GetIsolate", &GetIsolate).
          defineMethod("GetReturnValue", &GetReturnValue).
          store(&Class);
      }

    };

    class Void : public Base<void> {
    public:

      inline Void(v8::PropertyCallbackInfo<void> info) :
        Base<void>(info) {}

      inline Void(VALUE self) : Base<void>(self) {}

      /**
       * Call the Ruby code associated with this callback.
       *
       * Unpack the Ruby code, and the callback data from the C++
       * callback data, and then invoke that code.
       *
       * Note: This function implements the `v8::AccessorNameSetterCallback` API.
       */
      static void invoke(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
        v8::Isolate* isolate = info.GetIsolate();

        v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info.Data());
        v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::setter");

        VALUE code(External::unwrap(holder->GetHiddenValue(callback_key)));

        VALUE rb_property;
        if (property->IsSymbol()) {
          rb_property = Symbol(isolate, v8::Local<v8::Symbol>::Cast(property));
        } else {
          rb_property = String(isolate, property->ToString());
        }

        Unlocker unlock(info.GetIsolate());
        rb_funcall(
          code, rb_intern("call"), 3,
          rb_property,
          (VALUE)rr::Value::handleToRubyObject(isolate, value),
          (VALUE)Void(info)
        );
      }

      static inline void Init() {
        ClassBuilder("Void", PropertyCallbackInfo::Class, PropertyCallbackInfo::Class).
          defineMethod("This", &This).
          defineMethod("Data", &Data).
          defineMethod("GetIsolate", &GetIsolate).
          // defineMethod("GetReturnValue", &GetReturnValue).
          store(&Class);
      }

    };

    static VALUE Class;

    static void Init() {
      ClassBuilder("PropertyCallbackInfo").
        store(&Class);

      Value::Init();
      Void::Init();
    }

  };

}

#endif /* RR_PROPERTY_CALLBACK_H */
