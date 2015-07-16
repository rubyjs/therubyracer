// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_H
#define RR_PROPERTY_CALLBACK_H

namespace rr {

  typedef Wrapper<v8::PropertyCallbackInfo<v8::Value>> PropertyCallbackInfoWrapper;

  class PropertyCallbackInfo : public PropertyCallbackInfoWrapper {
  public:

    inline PropertyCallbackInfo(v8::PropertyCallbackInfo<v8::Value> info) :
      PropertyCallbackInfoWrapper(info) {}

    inline PropertyCallbackInfo(VALUE self) : PropertyCallbackInfoWrapper(self) {}

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
      holder->SetHiddenValue(data_key, Value(r_data));

      return holder;
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::AccessorNameGetterCallback` API.
     */
    static void invokeGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
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
      rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)PropertyCallbackInfo(info));
    }

    static VALUE This(VALUE self) {
      PropertyCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Object(info->GetIsolate(), info->This());
    }

    static VALUE Data(VALUE self) {
      PropertyCallbackInfo info(self);
      Isolate isolate(info->GetIsolate());
      Locker lock(isolate);

      v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info->Data());
      v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
      v8::Local<v8::Value> data(holder->GetHiddenValue(data_key));

      return Value::handleToRubyObject(info->GetIsolate(), data);
    }

    static VALUE GetIsolate(VALUE self) {
      PropertyCallbackInfo info(self);
      return Isolate(info->GetIsolate());
    }

    static VALUE GetReturnValue(VALUE self) {
      PropertyCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return ReturnValue(info->GetReturnValue());
    }

    static inline void Init() {
      ClassBuilder("PropertyCallbackInfo").
        defineMethod("This", &This).
        defineMethod("Data", &Data).
        defineMethod("GetIsolate", &GetIsolate).
        defineMethod("GetReturnValue", &GetReturnValue).
        store(&Class);
    }
  };
}

#endif /* RR_PROPERTY_CALLBACK_H */
